#include <unistd.h>
#include <netdb.h>
#include <iostream>
int main () {
  char name[256];
  gethostname(name, 256);
  std::cout<<name;
  
  return 0;
}
