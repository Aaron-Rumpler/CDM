cmake_minimum_required(VERSION 3.22)
project(CDM C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS ON)

add_compile_options(-Wall -Wextra $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)

find_library(CORE_FOUNDATION CoreFoundation REQUIRED)
find_library(IO_KIT IOKit REQUIRED)

add_executable(cdm main.c cdm.c sleep_assertion.c)
target_link_libraries(cdm ${CORE_FOUNDATION} ${IO_KIT})

add_executable(cdm_test cdm_test.c cdm.c)
target_link_libraries(cdm_test ${CORE_FOUNDATION} ${IO_KIT})

install(TARGETS cdm)
