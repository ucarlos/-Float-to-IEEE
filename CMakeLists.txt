cmake_minimum_required(VERSION 3.14)
project(Float_to_IEEE C)

set(CMAKE_C_STANDARD 11)

add_executable(Float_to_IEEE Float_to_IEEE.c Float_Driver.c)
target_link_libraries(Float_to_IEEE m)


