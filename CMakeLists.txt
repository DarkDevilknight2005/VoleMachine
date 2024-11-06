cmake_minimum_required(VERSION 3.29)
project(VoleMachine)

set(CMAKE_CXX_STANDARD 20)

add_executable(VoleMachine main.cpp
        lib/vole_machine.cpp
        lib/vole_machine.h)
