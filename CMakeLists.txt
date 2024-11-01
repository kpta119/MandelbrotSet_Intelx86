cmake_minimum_required(VERSION 3.20)
project(mandelbrot)

# assemble
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf64 -o <OBJECT> <SOURCE>")

# executable
add_executable(mandelbrot mandelbrot_set.asm main.c)

# allegro
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_primitives-5 allegro_image-5)

include_directories(${ALLEGRO5_INCLUDE_DIRS})
target_link_libraries(mandelbrot ${ALLEGRO5_LIBRARIES})
