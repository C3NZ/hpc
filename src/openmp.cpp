#include <iostream>
#include <chrono>

#include <omp.h>

void HelloOpenMP() {
  omp_set_num_threads(8);

  #pragma omp parallel 
  {
    std::cout << "Hello, OpenMP\n";
  }
}
