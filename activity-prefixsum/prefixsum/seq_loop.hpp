#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H
//#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <math.h>
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
		int nbthreads,
		int n,
	       std::function<void(TLS[])> before,
		std::function<void(int, TLS[], int)> f,
	       std::function<void(TLS[])> after
	       ) {
    
    TLS tls[n+1];
    std::vector<std::thread> threads;
   
    
    //before method
    before(tls);
    
    //create threads and partition work
      for (int j = 0; j < nbthreads; j++) {
	//Get curr iter value to pass as val instead of reference
	int curval = j;
	threads.emplace_back(std::thread([&](int k) {
					   //Calculate beginning and ends for thread iterations
					   int numiter = (n/increment)/nbthreads;
					   int beg = k * (numiter * increment) + 1;
					   int end = ((k + 1) == nbthreads) ? (n + 1) : (beg + (numiter*increment));
					   //For-loop for j # thread
					   for (size_t i=beg; i<end; i+= increment){					   
					     f(i, tls, end);
					  
					   }
					 }, curval));


      }
    

    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }
    //after method
    after(tls);
    
  }
  


  
};




#endif
