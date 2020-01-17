cmake_minimum_required(VERSION 3.12)
project(RD-Parser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)


FIND_PACKAGE(FLEX REQUIRED)

set(FLEX_SOURCE ${CMAKE_SOURCE_DIR}/Source/lex.yy.l)
set(FLEX_OUTPUT ${CMAKE_SOURCE_DIR}/Source/lex.yy.c)

add_custom_command(
        OUTPUT ${FLEX_OUTPUT}
        DEPENDS ${FLEX_SOURCE}
        COMMAND flex -o ${FLEX_OUTPUT} ${FLEX_SOURCE})

add_custom_target(FlexScanner DEPENDS ${FLEX_OUTPUT})

include_directories(Header)

add_executable(RD-Parser
        Source/main.cpp Header/lexer.h Header/Util/token.h Source/lexer.cpp
        Source/Util/token.cpp Header/Util/symbols.h Source/Util/symbols.cpp Header/parser.h Header/Util/AstNode.h
        Source/parser.cpp Source/Util/AstNode.cpp
        )

add_dependencies(RD-Parser FlexScanner)
