#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <thread>
#include <vector>

class SeqLoop {
public:

  int nbthreads;
  int n;
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
	       std::function<void(TLS&)> before,
	       std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after
	       ) {
    TLS tls;
    std::vector<std::thread> threads;
    size_t numiter = n/nbthreads;
    

    //before method
    before(tls);

    //create threads and partition work
    for (int j = 0; j < nbthreads; ++j) {
      threads.emplace_back(std::thread([&](size_t beg, size_t end) {
					 for (size_t i=beg; i<end; i+= increment){
					   f(i, tls);
					 }
				       }, (threads.size()*numiter), ((threads.size()+1)==nbthreads) ? n : ((threads.size() * numiter) + numiter)));
    }
    //wait for threads to finish
    for(auto& thread : threads){
      thread.join();
    }
    //after method
    after(tls);
  }



  /*passes in number of threads and n
  adjust size of threads if:
  1. There are more threads than num and num threads is greater than 1
  2. n is not evenly divisible by num threads
  */
  void manageThread(int numthread, int num){

    nbthreads = numthread;
    n = num;

    /*
      Deleted this code since it was giving me errors :'D 
      Not sure if we needed to do something along these lines, so hopefully that's okay

    if (((n/nbthreads) < 1) && (nbthreads > 1)) {
      while (((n % nbthreads) != 0) && (nbthreads > 1)) {
	nbthreads -= 1;
      }
    } else if ((n % nbthreads) != 0) {
      while (((n % nbthreads) != 0) && (nbthreads <= n)) {
	nbthreads += 1;
      }
      }*/

  }

  
};




#endif
