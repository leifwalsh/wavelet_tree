#include <iostream>

#include "wavelet_tree.hpp"

int main(void) {
  wavelet_tree t1("hello");
  t1.dprint();
  std::cout << (std::string) t1 << std::endl;
  wavelet_tree t2("abracadabra");
  t2.dprint();
  std::cout << (std::string) t2 << std::endl;
  wavelet_tree t3("peter piper picked a peck of pickled peppers");
  t3.dprint();
  std::cout << (std::string) t3 << std::endl;
  return 0;
}
