cmake_minimum_required(VERSION 3.17)
project(AKSOSeq0 C)

set(CMAKE_C_STANDARD 11)

add_executable(AKSOSeq0 main.c seq.h seq.c err.c err.h trie.h trie.c dsf.h dsf.c memory_tests.c memory_tests.h)