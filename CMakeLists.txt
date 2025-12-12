cmake_minimum_required(VERSION 3.16)
project(ardan C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB ENGINE_SRC engine/*.c engine/io/*.c engine/termicast/*.c engine/util/*.c)

add_executable(ardan ${ENGINE_SRC} main.c)

target_link_libraries(ardan PRIVATE m)
find_package(PNG REQUIRED)
target_link_libraries(ardan PRIVATE PNG::PNG)