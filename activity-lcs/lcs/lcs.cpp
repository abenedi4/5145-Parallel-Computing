#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "seq_loop.hpp"
#include <pthread.h>
#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif


int LCS(char* X, int m, char* Y, int n, int nbthreads) {
  //Create parfor object and storage for end result of LCS
  SeqLoop sl;
  int result = 0;

  //Parallel loop over tls matrix
    sl.parfor<int>(1, nbthreads, m, n,
		 [&](std::vector< std::vector < int > >& tls, int j) {
		   //Begin function - initialize all values in first row
		   //or first col to 0
		      if(j == 0) {
			for (int k = 0; k <= n; ++k) {
			  tls[j][k] = 0;
			}
			tls[j][0] = 0;
		      }
		 },
		   [&](int a, int b, std::vector<std::vector<int> >& tls) {
		     //Middle function - Check values to the left, above, and
		     //diagonally to see which value to take
		   if (X[a-1] == Y[b-1]) {
		     tls[a][b] = tls[a-1][b-1] + 1; 
		   } else {
		     tls[a][b] = std::max(tls[a-1][b], tls[a][b-1]);
		   }
		 },
		 [&](std::vector < std::vector < int > >& tls) {
		   //Return result of the max LCS
		   result = std::max(tls[m-1][n],std::max(tls[m][n-1], tls[m][n]));
		 });

  return result;
}


int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);
  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);


  
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n, nbthreads);

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkLCS(X, m, Y, n, lcs);
  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] X;
  delete[] Y;


  return 0;
}
