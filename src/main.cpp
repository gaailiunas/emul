#include <iostream>
#include <fstream>
#include <vector>

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

    std::vector<char> bootloader_data(bootloader_size);
    if (!bootloader_file.read(bootloader_data.data(), bootloader_size)) {
        std::cerr << "Failed to read bootloader file\n";
        return 1;
    }

    std::cout << "Bootloader file size: " << bootloader_size << "\n";

    return 0;
}

