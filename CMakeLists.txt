cmake_minimum_required(VERSION 3.17)
project(AKSOSeq0 C)

set(CMAKE_C_STANDARD 11)

add_executable(AKSOSeq0 main.c seq.h seq.c)