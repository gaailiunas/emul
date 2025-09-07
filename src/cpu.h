#ifndef EMUL_CPU_H
#define EMUL_CPU_H

#include <cstdint>

typedef struct cpu_s {
    std::uint64_t rax;
    std::uint64_t rbx;
    std::uint64_t rcx;
    std::uint64_t rdx;
    std::uint64_t rsi;
    std::uint64_t rdi;
    std::uint64_t rbp;
    std::uint64_t rsp;
    std::uint64_t r8;
    std::uint64_t r9;
    std::uint64_t r10;
    std::uint64_t r11;
    std::uint64_t r12;
    std::uint64_t r13;
    std::uint64_t r14;
    std::uint64_t r15;
} cpu_t;

#endif // EMUL_CPU_H
