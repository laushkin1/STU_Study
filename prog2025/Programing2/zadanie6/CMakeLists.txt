cmake_minimum_required(VERSION 3.27)
project(z6 C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -Wuninitialized -pedantic -g)

add_executable(z6 src/data.c src/z6.c)
target_include_directories(z6 PRIVATE include)
target_link_libraries(z6 -lncurses -lm)





