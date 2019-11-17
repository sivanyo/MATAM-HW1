cmake_minimum_required(VERSION 3.15)
project(hw1 C)

set(CMAKE_C_STANDARD 99)

add_executable(amount_set amount_set.c amount_set.h fromCourse/tests/amount_set_main.c fromCourse/tests/amount_set_tests.c fromCourse/tests/amount_set_tests.h)