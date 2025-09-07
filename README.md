# emul
An x86_64 emulator with the goal of booting up Arch Linux live-boot environment.

## Building
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Setting up Arch Linux image
1. Download the Arch Linux ISO.
2. Extract the ISO contents into the `image` directory at the root of this project.
Example structure:
```
emul/
├── build/
├── image/
│   ├── boot/
│   ├── arch/
│   ├── '[BOOT]'/
│   └── ...
├── src/
│   └── ...
└── CMakeLists.txt
```
