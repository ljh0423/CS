/*
This file contains main code which takes the first argument as a regex expression, and takes files as arguments.
If there are no file arguments, it takes input from stdin which outputs the line if the line containsMatch to the regex expression.
*/
#include <iostream>
#include <fstream>
#include <string>
#include "re.h"

int main(int argc, char *argv[]) {
	std::string s = std::string(argv[1]);
  auto re = CS246E::parseDisjunction(s);
  if (argc > 2) {
    for (int i = 2; i < argc; ++i) {
		  std::ifstream f{argv[i]};
      while (getline(f, s)) {
        if (containsMatch(re.get(),s)) std::cout<<s<<std::endl;
      }
	  }
  }
  else {
    while (getline(std::cin, s)) {
      if (containsMatch(re.get(),s)) std::cout<<s<<std::endl;
    }
  }
  return 0;
}
