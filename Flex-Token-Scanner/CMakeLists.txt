cmake_minimum_required(VERSION 3.12)
project(FlexTokenScanner)

set(CMAKE_C_STANDARD 11)

FIND_PACKAGE(FLEX REQUIRED)

set(FLEX_SOURCE ${CMAKE_SOURCE_DIR}/FlexTokenScanner.l)
set(FLEX_OUTPUT ${CMAKE_BINARY_DIR}/FlexTokenScanner.c)

add_custom_command(
        OUTPUT ${FLEX_OUTPUT}
        DEPENDS ${FLEX_SOURCE}
        COMMAND flex -o ${FLEX_OUTPUT} ${FLEX_SOURCE})

add_executable(FlexTokenScanner ${FLEX_OUTPUT})
