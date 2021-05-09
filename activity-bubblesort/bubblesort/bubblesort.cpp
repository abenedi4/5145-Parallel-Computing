#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "seq_loop.hpp"
#include <pthread.h>
#include <omp.h>
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(int arr[], int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
  
}

bool check(int arr[], int n) {

  for (int i = 0; i < n-1; ++i) {
    if (arr[i] > arr[i+1]) {
      return true;
    }

  }
  return false;

}


int main (int argc, char* argv[]) {
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
  SeqLoop sl;

  
  
  //split array up into nbthreads times
  //each thread will sort sub-arrays
  //sorting will alternate between every two even/odd pairs

  
  //insert sorting code here.
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int swapped  = 0;
  int counter = 0;
  bool even = true;
  
  while (counter < 3)  {
    //Only exit while loop after 3 iterations of no-swapping occurs
    //This is because we alternate between even/odd
  counter = (swapped == 0) ? (counter + 1) : counter;
  swapped = 0;

    int startingIndex = even ? 0 : 1;
    sl.parfor<int>(1, nbthreads, n, startingIndex,
		   [&](int& tls, int j) {
		      tls = arr[j];
		    },
		    [&](int i, int tls[]) {

			if (tls[i] > tls[i+1]) {
			  swap(tls, i, i+1);
			  swapped = 1;
			} 
		    },
		   [&](int& tls, int j ) {
		      arr[j] = tls;
		      
		    }
	    );

  
    even = !even;
  }



  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;



  checkMergeSortResult (arr, n);
  std::cerr<<elpased_seconds.count()<<std::endl;
  
  delete[] arr;

  return 0;
}


