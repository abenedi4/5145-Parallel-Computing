#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <iostream>
#include "seq_loop.hpp"
#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {


  
  //Create pointers to each function
  auto fptr1 = f1;
  auto fptr2 = f2;
  auto fptr3 = f3;
  auto fptr4 = f4;
  
  //Store function pointers in a list
  std::vector<float (*)(float, int)> funct_list = {fptr1, fptr2, fptr3, fptr4};

  
  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }


    //Convert command line arguments to store in variables
  int functionId = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[7]);
  SeqLoop sl;
  //Get time start
  auto start =std::chrono::steady_clock::now();
  
   float sum = 0;
   //manage number of threads if unevenly divisble, and also pass in
   //nbthreads and n to instance of seqloop
   sl.manageThread(nbthreads, n);   
   sl.parfor<float>(1,
		    [&](float& tls) {
		      tls = 0;
		    },
		    [&](int i, float& tls) {
		      float x = (a + ((float)i + 0.5)) * ((b-a)/n);
		     
		      tls += (float)funct_list[functionId - 1](x , intensity);
		    },
		    [&](float& tls) {
		     
		      sum += tls;
		    }
	    );
   //calculate answer after obtaining sum from parfor
  float answer = ((b-a)/n) * sum;

  //Get time end and calculate duration of time elapsed
  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;

  std::cout<<answer<<"\n";
  std::cerr << time_elapsed.count();
  return 0;
}
