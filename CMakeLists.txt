cmake_minimum_required(VERSION 3.22)
project(CDM C)

set(CMAKE_C_STANDARD 17)

find_library(CORE_FOUNDATION CoreFoundation REQUIRED)
find_library(IO_KIT IOKit REQUIRED)

add_executable(cdm main.c cdm.c sleep_assertion.c)
target_compile_options(cdm PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(cdm ${CORE_FOUNDATION} ${IO_KIT})

install(TARGETS cdm)
