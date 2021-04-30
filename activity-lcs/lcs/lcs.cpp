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

  int** C = new int*[m+1];
  SeqLoop sl;
  int result = 0;
  sl.parfor<int>(1, nbthreads, m, n,
		 [&](std::vector< std::vector < int > >& tls, int j) {
		   std::cout<<"Start before\n";
		      if(j == 0) {
			for (int k = 0; k <= n; ++k) {
			  tls[j][k] = 0;
			}
			tls[j][0] = 0;
		      }
		   std::cout<<"End before\n";
		 },
		 [&](int i, int x, int z, std::vector<std::vector<int> >& tls) {                   std::cout<<"Start middle\n";
		   int a = z + i;
		   int b = x - i;
		   std::cout<<"A is: "<<a<<" B is: "<<b<<"\n";
		   std::cout<<"N is: "<<n<<" M is: "<<m<<"\n";
		   if (X[a-1] == Y[b-1]) {
		     tls[a][b] = tls[a-1][b-1] + 1; 
		   } else {
		     tls[a][b] = std::max(C[a-1][b], C[a][b-1]);
		   }
		   std::cout<<"End middle\n";
		 },
		 [&](std::vector < std::vector < int > >& tls) {
		   std::cout<<"Start after\n";
		   //get result to return
		   result = tls[m][n];
		   std::cout<<"End after\n";
		 });

  
	      /* for (int i=0; i<=m; ++i) {
		 C[i] = new int[n+1];
		 C[i][0] = 0;
		 }
		 for (int j=0; j<=n; ++j) {
		 C[0][j] = 0;
		 }

  for (int a=1; a<=m; ++a) {
    for (int b=1; b<=n; ++b) {
      if (X[a-1] == Y[b-1]) {
        C[a][b] = C[a-1][b-1] + 1; 
      } else {
        C[a][b] = std::max(C[a-1][b], C[a][b-1]);
      }
    }
  }

  int result = C[m][n];
	     
	      
  for (int i=0; i<=m; ++i) { 
    delete[] C[i];
  }
  delete[] C;
	      */
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

  
  //insert LCS code here.
  int result = -1; // length of common subsequence

  for (int row = 0; row < m; ++row) {
     std::cout<<X[row]<<", ";

  }
 std::cout<<"\n";
  for (int col = 0; col < n; ++col) {
      std::cout<<Y[col]<<", ";

    }

  
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
