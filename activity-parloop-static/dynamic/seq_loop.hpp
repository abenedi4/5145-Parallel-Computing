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
		int granularity,
	       std::function<void(std::reference_wrapper<TLS>)> before,
	       std::function<void(int, std::reference_wrapper<TLS>)> f,
	       std::function<void(std::reference_wrapper<TLS>)> after
	       ) {
    

    std::vector<std::thread> threads;

    size_t numiter = n/granularity;
    bool tasks[n] = {false};
    TLS tls[n];

    //std::cout<<numiter<<" is num iter\n";
    for (int k = 0; k < n; ++k) {
      before(std::ref(tls[k]));

    }

    //create threads
     
      for (int j = 0; j < nbthreads; ++j) {
	threads.emplace_back(std::thread([&]() {
					   //Continue looping and assigning new tasks until tasks are done
					   
					   for (int iter = 0; iter < n; ++iter) {
					     //Create new task and set flag to not done
					     bool done = false;
					     int startingIter = selectIter(tasks, n,  granularity);
					     int flag = newTask(std::ref(tls[startingIter]),
						     startingIter,
						     f,
						     numiter, &done,
						     increment);
					     if (flag == -1) break;
					     while (!done);
					     //End of task once flag set to true
					     
					   }

					   
					 }
));

      }

     
			        

  

    
    //wait for threads to finish
    for (auto& thread : threads){
      thread.join();
    }

    //Aggregate sum after threads are complete
    for (int k = 0; k < n; ++k){
      after(std::ref(tls[k]));
  }


}
  


  
  int selectIter(bool tasks[], int size, int granularity) {
   
    std::lock_guard<std::mutex> lg(mut);
    for (int i = 0; i < size; i += (size / granularity)) {
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
	int end = beg + numiter;
	//std::cout<<"beg: "<<beg<<"\n";
	for (size_t i=beg; i<end; i+= increment){
	  f(i, temptls);
	  
	}
				      
	//Set flag back in thread loop to true (done)
	*done = true;
	return startingIter;
  }
  
  
};




#endif
