#include <iostream>
#include <string>
#include <cstdio>

std::string GetCPUName() {
  int data[3] = { 0 };

  asm(
    "mov $0x0, %%eax\n\t"
    "cpuid\n\t"
    "mov %%ebx, %0\n\t"
    "mov %%edx, %1\n\t"
    "mov %%ecx, %2\n\t"
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
}

int main(int argc, char* argv[]) {
  assembler();
}
