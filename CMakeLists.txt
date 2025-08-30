cmake_minimum_required(VERSION 4.0)
project(rv32_vm C)

include(cmake/warnings.cmake)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(rv32_vm rv32_vm.c)
set_warnings(rv32_vm ON)
