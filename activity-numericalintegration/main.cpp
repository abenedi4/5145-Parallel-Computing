#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <vector>
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

  //Check if we have not received the right # of args
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  //Convert command line arguments to store in variables
  int functionId = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);

  //Get time start
  auto start =std::chrono::steady_clock::now();
  
  //Calculations
  float sum = 0;
  for(int i = 0; i < n; ++i) {
    float x = (a + ((float)i + 0.5)) * ((b-a)/n);
    sum = sum + (float)funct_list[functionId - 1](x , intensity);
  }

  float answer = ((b-a)/n) * sum;
  
  //Get time end and calculate duration of time elapsed
  auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;

  //Print results
  std::cout << answer;
  std::cerr << time_elapsed.count();
  return 0;
}
