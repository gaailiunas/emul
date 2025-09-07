#include "emul.h"
#include <capstone/x86.h>
#include <fstream>
#include <stdexcept>
#include <capstone/capstone.h>

#include <iostream>

Emulator::Emulator(const char *bootloader_path) : m_ram(128 * 1024 * 1024), m_state{0}
{
    std::ifstream bootloader_file(bootloader_path, std::ios::binary | std::ios::ate);
    if (!bootloader_file) {
        throw std::runtime_error("Failed to open the bootloader file");
    }

    m_bootloader_size = bootloader_file.tellg();
    bootloader_file.seekg(0, std::ios::beg);

    if (m_bootloader_size > m_ram.size()) {
        throw std::runtime_error("Bootloader does not fit inside the allocated RAM");
    }

    if (!bootloader_file.read(reinterpret_cast<char *>(m_ram.data() + 0x7c00), m_bootloader_size)) {
        throw std::runtime_error("Failed to read the bootloader file");
    }
}

Emulator::~Emulator(void) {}

bool Emulator::run(void)
{
    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_16, &handle) != CS_ERR_OK) {
        return false;
    }

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    cs_insn *insn = nullptr;
    std::size_t count = cs_disasm(handle, m_ram.data() + 0x7c00, m_bootloader_size, 0, 0, &insn);
    if (!count) {
        std::cerr << "Failed to disassemble bootloader\n";
        cs_close(&handle);
        return false;
    }

    for (std::size_t i = 0; i < count; i++) {
        cs_x86 &details = insn[i].detail->x86;

        switch (insn[i].id) {
            case X86_INS_INT: {
                if (details.op_count > 0 && details.operands[0].type == X86_OP_IMM) {
                    std::uint64_t int_number = details.operands[0].imm;
                    std::cout << "interrupt_number: 0x" << std::hex << int_number << "\n";
                }
                break;
            }
            case X86_INS_LJMP: {
                std::uint64_t linear_addr = get_linear_address(details);
                std::cout << "long jump to: 0x" << std::hex << linear_addr << std::endl;
                break;
            }
            default: {
                break;
            }
        }
    }

    cs_free(insn, count);
    cs_close(&handle);
    return true;
}

std::uint64_t Emulator::get_linear_address(const cs_x86 &x86)
{
    if (x86.op_count == 2) {
        auto &seg = x86.operands[0];
        auto &off = x86.operands[1];
        if (seg.type == X86_OP_IMM && off.type == X86_OP_IMM) {
            return seg.imm * 16 + off.imm;
        }
    }
    else if (x86.op_count == 1) {
        auto &off = x86.operands[0];
        if (off.type == X86_OP_IMM) {
            return off.imm;
        }
    }
    return 0;
}
