cmake_minimum_required(VERSION 3.10)
project(emul)

set(CMAKE_CXX_STANDARD 20)

find_package(capstone REQUIRED)

add_executable(emul
    src/main.cpp
)

target_link_libraries(emul capstone::capstone_static)
