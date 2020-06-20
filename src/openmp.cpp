#include "openmp.h"

#include <cmath>
#include <iostream>
#include <chrono>

#include <omp.h>

// Hello, OpenMP! Demonstrate a code block being run across multiple (in our 
// case, 8) threads.
void HelloOpenMP() {
  omp_set_num_threads(8);

  #pragma omp parallel 
  {
    std::cout << "Hello, OpenMP!\n";
  }
}

// Illustrate the effects of using OpenMP to run code within a for loop in 
// parallel. This directive will only apply parallelization to the for loop and
// because the for loop doesn't share data across outside of it's local scope, 
// we don't have to worry about locking or race conditions across threads.
void OpenMPIterate() {
  const int length = 1024 * 1024 * 64;
  float* a = new float[length];
  float* b = new float[length];
  float* c = new float[length];
  float* result = new float[length];

  #pragma omp parallel for
  for (int i = 0; i < length; ++i) {
    result[i] = a[i] + b[i] * std::erf(c[i]);
  }

  delete[] a;
  delete[] b;
  delete[] c;
  delete[] result;

}

// Illustrate the effects of splitting OpenMP up into different sections. Each
// of the sections will be run within separate threads.
void OpenMPSections() {
  #pragma omp parallel sections 
  {
    #pragma omp section
    {
      for (int i = 0; i < 1000; ++i) {
        std::cout << i;
      }
    }

    #pragma omp section
    {
      for (int i = 0; i < 1000; ++i) {
        std::cout << static_cast<char>('a' + (i % 26));
      }
    }
  }
}

void OpenMPSingleMaster() {
  std::cout << std::endl;

  #pragma omp parallel
  {
    #pragma omp single 
    {
      std::printf("Gathering input on thread: %d\n", omp_get_thread_num());
    }

    std::printf(
        "Parallel process happening on thread: %d\n", omp_get_thread_num());

    #pragma omp barrier

    #pragma omp master 
    {
      std::printf("Printing output on thread: %d\n", omp_get_thread_num());
    }
  }
}

void OpenMPSync() {
  printf("\nATOMIC\n");

  int sum = 0;

  #pragma omp parallel for num_threads(128)
  for (int i = 0; i < 100; ++i) {
    ++sum;
  }

  std::cout << sum;
}


void RunOpenMPExamples() {
  HelloOpenMP();
  OpenMPIterate();
  OpenMPSections();
  OpenMPSingleMaster();
  OpenMPSync();
}
