#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <capstone/capstone.h>

std::vector<std::uint8_t> ram(128 * 1024 * 1024); // 128MB

int main(void)
{
    // assuming we are at build/
    std::ifstream bootloader_file("../image/[BOOT]/1-Boot-NoEmul.img", std::ios::binary | std::ios::ate);
    if (!bootloader_file) {
        std::cerr << "Failed to open bootloader file\n";
        return 1;
    }

    std::streamsize bootloader_size = bootloader_file.tellg();
    bootloader_file.seekg(0, std::ios::beg);

    if (!bootloader_file.read(reinterpret_cast<char *>(ram.data() + 0x7c00), bootloader_size)) {
        std::cerr << "Failed to read bootloader file\n";
        return 1;
    }

    std::cout << "Bootloader file size: " << bootloader_size << "\n";

    csh handle;
    if (cs_open(CS_ARCH_X86, CS_MODE_16, &handle) != CS_ERR_OK) {
        return 1;
    }

    cs_insn *insn = nullptr;
    std::size_t count = cs_disasm(handle, ram.data() + 0x7c00, bootloader_size, 0, 0, &insn);
    if (!count) {
        std::cerr << "Failed to disassemble bootloader\n";
        cs_close(&handle);
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        std::cout << std::hex << insn[i].address << ":\t" << insn[i].mnemonic << "\t" << insn[i].op_str << "\n";
    }

    cs_free(insn, count);
    cs_close(&handle);
    return 0;
}

