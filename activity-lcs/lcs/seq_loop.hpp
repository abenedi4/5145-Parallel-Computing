#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H
//#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <pthread.h>
class SeqLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  void parfor ( size_t increment,
		int nbthreads, int m, int n, 
		std::function<void(std::vector< std::vector < TLS > >&, int)> before,
		std::function<void(int, int, std::vector< std::vector < TLS > >&)> f,
	       std::function<void(std::vector< std::vector < TLS > >&)> after
	       ) {
    std::vector< std::vector < TLS > > tls((m+1), std::vector < TLS >((n+1)));
    
    for (int j = 0; j <= m; ++j) {
      before(tls, j);
    }


    //Set row and iterate over columns
    //Row will iterate after we reach max column (once we are done iterating
    //antidiagonals over horizontally, move downwards over rows
    int row = 1;
    for (int col=1; col<=n && row <= m; ++col) {

      //Create local vector to store threads temporarily
      std::vector<std::thread> threads;

      //Get size of the diagonal to iterate over and partition between
      //number of possible threads
      int rowSize = (m == row) ? 1 : (m - row);
      int iterSize = std::min(col, rowSize);
      int tempNbThrds = (nbthreads > iterSize) ? iterSize : nbthreads;
      int numiter = iterSize / tempNbThrds;

      //Create threads and iterate over the diagonal
      for (int j = 0; j < tempNbThrds; ++j) {
	
	int temp_beg = j * numiter;
	int temp_end = ((j + 1) == tempNbThrds) ? iterSize : (temp_beg + numiter);
	
	threads.emplace_back(std::thread([&](size_t beg, size_t end) {
					   for (size_t i=beg; i<end; i+= increment){
					     int temp_col = col - i;
					     int temp_row = row + i;
					     
					       f(temp_row, temp_col, tls);
					   }
					 }, temp_beg, temp_end
	    ));


      }
      //wait for threads to finish current antidiagonal
      for (auto& thread : threads){
	if(thread.joinable()){
	  thread.join();
	}
      }

      //If we have reached the max # columns, start iterating rows
      if ( col == n ) {
	col--;
	row++;
      }    
     }

      after(tls);
      

    
  }

  
};




#endif
