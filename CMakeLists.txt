cmake_minimum_required(VERSION 3.15)
project(hw1 C)

set(CMAKE_C_STANDARD 99)

add_executable(amount_set amount_set.c amount_set.h tests/amount_set_main.c tests/amount_set_tests.c tests/amount_set_tests.h)
add_executable(amount_set_local main.c amount_set.c amount_set.h)
add_executable(amount_set_noa noa_amount_set_tests.c amount_set.c amount_set.h)
add_executable(amount_set_safi safi_tests_amount_set.c amount_set.c amount_set.h)
add_executable(amount_set_tests amount_set_tests_custom.c amount_set amount_set.c amount_set.h)



add_executable(matamazom_local main2.c matamazom.c matamazom.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_local m)
add_executable(matamazom_noa noa_matamazom_tests.c matamazom.c matamazom.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_noa m)
add_executable(matamazom_yonatana matamazom.c matamazom.h test_matamazom_yonatan_and_hen.c tests/matamazom_tests.c tests/matamazom_tests.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_yonatana m)
add_executable(matamazom_real matamazom.c matamazom.h tests/matamazom_main.c tests/matamazom_tests.c tests/matamazom_tests.h amount_set.c amount_set.h matamazom_print.c matamazom_print.h)
target_link_libraries(matamazom_real m)