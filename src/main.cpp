#include "emul.h"
#include <iostream>

int main(void)
{
    Emulator emul("../image/[BOOT]/1-Boot-NoEmul.img");
    bool status = emul.run();
    std::cout << "Emulator return status: " << status << "\n";
    return 0;
}

