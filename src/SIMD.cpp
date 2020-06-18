#include <iostream>
#include <string>
#include <cstdio>

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

void assembler() {
  std::cout << "CPU is " << GetCPUName() << std::endl;

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

int main(int argc, char* argv[]) {
  assembler();
}
