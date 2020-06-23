#include <iostream>
#include <string>
#include <cstdio>
#include <Vc/Vc>

#include "openmp.h"
#include "mpitest.h"

// Retrieves the CPU brand by executing configuring %eax to a specific register
// and then running the cpuid program.
std::string GetCPUName() {
  int data[3] = { 0 };

  asm(
    "mov $0x0, %%eax;"
    "cpuid;"
    "mov %%ebx, %0;"
    "mov %%edx, %1;"
    "mov %%ecx, %2;"
    : "=b" (data[0]),
      "=d" (data[1]),
      "=c" (data[2])
  );

  return std::string(reinterpret_cast<const char*>(data));
}

// Multiply two float arrays using SIMD extensions. Each of the float arrays
// are packed into 128 bit registers and then multiplied in parallel.
void MultiplyWithSIMDAssembly() {
  float f1[] = { 1.f, 2.f, 3.f, 4.f };
  float f2[] = { 5.f, 4.f, 3.f, 2.f };

  float result[4] = { 0.0f };
  asm(
    "movups %1, %%xmm0;"
    "movups %2, %%xmm1;"
    "mulps %%xmm0, %%xmm1;"
    "movups %%xmm1, %0;"
    : "=m"(result)
    : "m" (f1), "m" (f2)
    : "%xmm0", "%xmm1"
  );

  for (int i = 0; i < 4; i++) {
    std::cout << result[i] << "\n";
  }
}

// This is doing the same set of operations that were done in the previous 
// function with inline assembly.
void MultiplyWithIntrinsics() {
  float f1[] = { 1.f, 2.f, 3.f, 4.f };
  float f2[] = { 5.f, 4.f, 3.f, 2.f };
  uint32_t indices[] = { 0, 1, 2, 3 };

  Vc::float_v vec1(f1, indices);
  Vc::float_v vec2(f2, indices);
  Vc::float_v result = vec1 * vec2;

  std::cout << result << std::endl;
}

void assembler() {
  std::cout << "CPU is " << GetCPUName() << std::endl;
  MultiplyWithSIMDAssembly();
  MultiplyWithIntrinsics();
}

int main(int argc, char* argv[]) {
  // assembler();
  // RunOpenMPExamples();
  RunMPIExamples(argc, argv);
}
