cmake_minimum_required(VERSION 3.24)
project(test C)

set(CMAKE_C_STANDARD 11)

add_executable(test main.c util.c util.h m.c m.h cam.c cam.h)
target_link_libraries(test m GLEW GL glut)
