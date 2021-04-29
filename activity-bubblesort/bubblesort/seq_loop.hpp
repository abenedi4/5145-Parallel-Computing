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
		int nbthreads,
		int n,
		int startingIndex,
		std::function<void(TLS&, int)> before,
	       std::function<void(int, TLS[])> f,
		std::function<void(TLS&, int)> after
	       ) {
    
    TLS tls[n];
    std::vector<std::thread> threads;
    size_t numiter = n/nbthreads;
   


    for (int j = 0; j < n; ++j) {
      before(std::ref(tls[j]), j);
    }

    //create threads and partition work
    for (int j = 0; j < nbthreads; ++j) {
    //If more threads than available pairs
      if (j > (n - startingIndex / 2)) {
	break;
      }
      threads.emplace_back(std::thread([&](size_t beg, size_t end) {
					 for (size_t i=beg; i<end-1; i+= increment){
					   f(i, tls);
					 }
				       },getBeg(j, numiter, startingIndex),
	  getIndex(getBeg(j, numiter, startingIndex),((j * numiter) + numiter + startingIndex), j, nbthreads, n)));


    }

    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }

    for (int k = 0; k < n; ++k){
      after(std::ref(tls[k]), k);
    }
   
  }
  

  int getBeg(int j, int numiter, int startingIndex) {

    if (j > 0) {
      return (j*numiter) + startingIndex - (numiter % 2);
    } else {
      return (j*numiter) + startingIndex;
    }
  }
  int getIndex(int beg, int end, int j, int nthreads, int n) {
    if ((j+1) == nthreads) {
      return n;
    } else if ((end - beg) % 2 != 0) {
      return (end - 1);
    } else {
      return end;
    }

  }
  
};




#endif
