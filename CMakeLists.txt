cmake_minimum_required(VERSION 3.16)
project(mygame VERSION 1.0 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the src folder CMake
add_subdirectory(src)