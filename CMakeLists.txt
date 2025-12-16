cmake_minimum_required(VERSION 3.16)
project(ardan C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# engine src
file(GLOB ENGINE_SRC
    engine/*.c
    engine/io/*.c
    engine/io/ae_sdl/*.c
    engine/io/terminal/*.c
    engine/render/termicast/*.c
    engine/util/*.c
)

add_executable(ardan main.c ${ENGINE_SRC})


target_include_directories(ardan PRIVATE engine)

# sdl2 option
option(AE_ENABLE_SDL2 "Enable SDL2-based IO module" ON)

if(AE_ENABLE_SDL2)
    find_package(SDL2 REQUIRED)
    target_link_libraries(ardan PRIVATE SDL2::SDL2)
endif()

# math lib
if(UNIX AND NOT APPLE)
    target_link_libraries(ardan PRIVATE m)
endif()

# png
find_package(PNG REQUIRED)
target_link_libraries(ardan PRIVATE PNG::PNG)