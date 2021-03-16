#include <iostream>
#include <thread>
#include <vector>
//Method to say minion greeting based on x iteration
void greeting(int x) {
  if(x < 41) {
    std::cout<<"Hello! I am minion "<<x<<"\n";
  } else {
    std::cout<<"Hello Minions! I am the Overlord!"<<"\n";
  }
}

int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  //Convert arg to int
  int num = atoi(argv[1]);

  //Create vector of threads to store minions
  std::vector<std::thread> minions;

  //Loop and create threads
  for(int i = 0; i<num; ++i) {
    minions.emplace_back(std::thread(greeting, (i + 1)));
}

  //Wait for threads to finish
  for(auto& thread : minions){
    thread.join();
}

  //Print last Overlord message
  std::thread ovld(greeting, 41);
  ovld.join();
  return 0;
}
