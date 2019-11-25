cmake_minimum_required(VERSION 3.15)
project(hw1 C)

set(CMAKE_C_STANDARD 99)

add_executable(amount_set amount_set.c amount_set.h fromCourse/tests/amount_set_main.c fromCourse/tests/amount_set_tests.c fromCourse/tests/amount_set_tests.h)
add_executable(amount_set_local main.c amount_set.c amount_set.h)

add_executable(matamazom_local main2.c matamazom.c matamazom.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_local m)
add_executable(matamazom_real matamazom.c matamazom.h fromCourse/tests/matamazom_main.c fromCourse/tests/matamazom_tests.c fromCourse/tests/matamazom_tests.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_real m)