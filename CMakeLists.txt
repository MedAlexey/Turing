cmake_minimum_required(VERSION 3.10)
project(Turing C)

set(CMAKE_C_STANDARD 11)

add_executable(Turing main.c structures.h infoInput.c infoInput.h strcut.c strcut.h infoOutput.c infoOutput.h)