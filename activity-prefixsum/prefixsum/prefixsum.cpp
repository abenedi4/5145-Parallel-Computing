#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "seq_loop.hpp"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  int * arr = new int [n];
  generatePrefixSumData (arr, n);

  int * pr = new int [n+1];
  SeqLoop sl;

  //Start time
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  //Loop outer # times to calculate steps for ea inner loop
  for (int d = 0; d <= (int)log2(n-1); d++) {
    //parallel inner for loop for summing pairs
    sl.parfor<int>(pow(2, (d+1)), nbthreads, n,
		   [&](int tls[]) {
		     tls[0] = 0;
		     //initialize tls array based on either arr (first iteration) or pr
		     if(d == 0) {
		       for (int j = 1; j <= n; j++) {
			 tls[j] = arr[j-1];
		       }
		     } else {
		       for (int j = 0; j <= n; j++) {
			 tls[j] = pr[j];
		       }
		     }
		   },
		   [&](int i, int tls[], int end) {
		     //Check that pairs aren't out of bounds, sum if in bounds
		     if ((i+(int)pow(2, (d+1))-1) < end && (i+(int)pow(2, d)-1) < end) {
		       tls[i+(int)pow(2, (d+1))-1] = tls[i+(int)pow(2, d)-1] + tls[i+(int)pow(2, (d+1))-1];
		     }
		   },
		   [&](int tls[]) {
		     //Set pr array to new summed values
		     for (int i = 0; i <= n; i++) {
		       pr[i] = tls[i];
		     }
		   }
		   );
  }
 
  
  //Stop time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  //Check results
  checkPrefixSumResult(pr, n);

  delete[] arr;

  return 0;
}
