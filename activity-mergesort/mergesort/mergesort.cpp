#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "seq_loop.hpp"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


void merge(int  arr[], int l, int mid, int r) {
  
#if DEBUG
  std::cout<<l<<" "<<mid<<" "<<r<<std::endl;
#endif

  // short circuits
  if (l == r) return;
  if (r-l == 1) {
    if (arr[l] > arr[r]) {
      int temp = arr[l];
      arr[l] = arr[r];
      arr[r] = temp;
    }
    return;
  }

  int i, j, k;
  int n = mid - l;
  
  // declare and init temp arrays
  int *temp = new int[n];
  for (i=0; i<n; ++i)
    temp[i] = arr[l+i];

  i = 0;    // temp left half
  j = mid;  // right half
  k = l;    // write to 

  // merge
  while (i<n && j<=r) {
     if (temp[i] <= arr[j] ) {
       arr[k++] = temp[i++];
     } else {
       arr[k++] = arr[j++];
     }
  }

  // exhaust temp 
  while (i<n) {
    arr[k++] = temp[i++];
  }

  // de-allocate structs used
  delete[] temp;

}


void mergeSort(int arr[], int i, int n, int j,  int start, int end) {

  int m, r = 0;

  for (int l = 0; l < end-1; l += (i * 2)) {

    m = std::min((l+i) - 1, n - 1);
    r = std::min((l + (2 * i) - 1), n - 1);
    merge(arr, l, m, r);
		 
  }
  
}




int main (int argc, char* argv[]) {
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);


     for(int i = 0; i < n; i++) {
      std::cout<<arr[i]<<", ";
    }
    std::cout<<"\n";

  
  //insert sorting code here.

   // begin timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  SeqLoop sl;


    sl.parfor<int>(1, nbthreads, n,
		   [&](int tls[]) {
		     std::cout<<"In begin\n";
		      for (int i = 0; i < n; ++i) {
			tls[i] = arr[i];
		      }
		    },
		   [&](int i, int tls[], int j) {
		     std::cout<<"In middle\n";
		     std::cout<<"J = "<<j<<"\n";
		     int numiter = n / nbthreads;
		     int start = j * numiter;
		     int end = start + numiter;
		     std::cout<<"Start: "<<start<<" End: "<<end<<"\n";
		     if ( end <= n ){

		       int m, r = 0;

		       for (int l = 0; l < end-1; l += (i * 2)) {

			 m = std::min((l+i) - 1, n - 1);
			 r = std::min((l + (2 * i) - 1), n - 1);
			 merge(arr, l, m, r);
		 
		       }
		     }
		    },
		    [&](int tls[]) {
		      std::cout<<"In after\n";
		      merge(tls, 0, (n-1)/2, n-1);
		      for (int i = 0; i < n; ++i) {
			arr[i] = tls[i];
		      }
		    }
	    );

    for(int i = 0; i < n; i++) {
      std::cout<<arr[i]<<", ";
    }
    std::cout<<"\n";
  // end timing
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  // display time to cerr
  std::cerr<<elpased_seconds.count()<<std::endl;

  
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
