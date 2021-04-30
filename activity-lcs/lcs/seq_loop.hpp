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
		std::function<void(int, int, int, std::vector< std::vector < TLS > >&)> f,
	       std::function<void(std::vector< std::vector < TLS > >&)> after
	       ) {
    
    std::vector< std::vector < TLS > > tls((m+1), std::vector < TLS >(n+1));
    std::vector<std::thread> threads;
    size_t numiter = n/nbthreads;
   


    for (int j = 0; j <= m; ++j) {
      before(tls, j);
    }

    /*
  for (int a=1; a<=m; ++a) {
    for (int b=1; b<=n; ++b) {
      if (X[a-1] == Y[b-1]) {
     */
    //x < length of Y or b, z < length of X or a
    int z = 1;
    for (int x = 1; x <= n && z <= m; ++x) {
      //create threads and partition work
      for (int j = 0; j < nbthreads; ++j) {
	//before method

	threads.emplace_back(std::thread([&](size_t beg, size_t end) {
					   for (size_t i=beg; i<end; i+= increment){					   
					     f(i, x, z, tls);
					  
					   }
					 }, 0, getEnd(m, x, z)
	    ));


      }
      //If x hits boundary, move to next row
      if ( x+1 == n && z < m){
	x--;
	z++;
      }
    }
    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }


    after(tls);
    
  }
  

  int getEnd(int m, int x, int z) {
    std::cout<<"In get end\n";
    //get new bound for end iteration
    if (x < (m - z)) {
      return x + 1;
    } else {
      return (m-z) + 1;
    }
  }

  
};




#endif
