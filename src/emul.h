#ifndef EMUL_H
#define EMUL_H

#include "cpu.h"
#include <vector>
#include <cstdint>

#include <capstone/capstone.h>

class Emulator {
public:
    Emulator(const char *bootloader_path);
    ~Emulator(void);

    bool run(void);

private:
    std::vector<std::uint8_t> m_ram;
    cpu_t m_state;
    std::size_t m_bootloader_size;

    std::uint64_t get_linear_address(const cs_x86 &x86);
};

#endif // EMUL_H
