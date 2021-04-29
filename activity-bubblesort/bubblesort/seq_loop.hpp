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
    //before method

      threads.emplace_back(std::thread([&](size_t beg, size_t end) {
					 for (size_t i=beg; i<end-1; i+= increment){					   
					   f(i, tls);
					  
					 }
				       }, (j*numiter) + startingIndex, ((j+1)==nbthreads) ? n : ((j * numiter) + numiter + startingIndex)));


    }

    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }

    for (int k = 0; k < n; ++k){
      after(std::ref(tls[k]), k);
    }
  }
  


  
};




#endif


/*
#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H
//#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
class SeqLoop {
public:
  std::mutex mut;
  int tasksdone = 0;
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
    

    std::vector<std::thread> threads;

    //size_t numiter = n/granularity;
    bool tasks[n] = {false};
    TLS tls[n];


    for (int j = 0; j < n; ++j) {
      before(std::ref(tls[j]), j);
    }

    //create threads
     
      for (int j = 0; j < nbthreads; ++j) {
	threads.emplace_back(std::thread([&]() {
					   //Continue looping and assigning new tasks until tasks are done
					   
					   for (int iter = 0; iter < n; ++iter) {
					     //Create new task and set flag to not done
					     bool done = false;
					     int startingIter = selectIter(tasks, n,  increment, startingIndex);
					     //int flag = newTask(std::ref(tls[startingIter]),
					//	     startingIter,
					//	     f,
					//	     numiter, &done,
					//	     increment);
					     if (startingIter != -1) {
					       f(startingIter, tls);
					     }
					     else {
					       break;
					     }
					     while (!done);
					     //End of task once flag set to true
					     
					   }
					 }));

					   
      }


      

     
			        

  

    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }


    for (int k = 0; k < n; ++k){
      after(std::ref(tls[k]), k);
    }


}
  


  
  int selectIter(bool tasks[], int size, int increment, int startingIndex) {
   
    std::lock_guard<std::mutex> lg(mut);
    for (int i = startingIndex; i < size; i += increment) {
      if (tasks[i] == false) {
	//Set iteration as taken
	tasks[i] = true;
	//Return index of selected task
	return i;
      }
      
    }
    return -1;
  }

     template<typename TLS>
  int newTask(std::reference_wrapper<TLS> temptls, int startingIter,
	      std::function<void(int, std::reference_wrapper<TLS>)> f, int numiter, bool *done, size_t increment) {


    //Find available iteration


   

	//Get begining and end iterations for thread
	int beg = startingIter;
	int end = beg + 1;
	//std::cout<<"beg: "<<beg<<"\n";
	if (startingIter != -1) {
	  for (size_t i=beg; i<end; i+= 1){
	    f(i, temptls);
	  
	  }
	}		      
	//Set flag back in thread loop to true (done)
	*done = true;
	return startingIter;
    }
  
  
};




#endif
  
*/
