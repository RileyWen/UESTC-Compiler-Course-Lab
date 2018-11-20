cmake_minimum_required(VERSION 3.12)
project(ToyCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)


FIND_PACKAGE(FLEX REQUIRED)
IF (FLEX_FOUND)
    SET(Flex_Source ${CMAKE_SOURCE_DIR}/Source/lex.yy.c)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${Flex_Source}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${Flex_Source}
            ${CMAKE_SOURCE_DIR}/Source/lex.yy.l
            COMMENT "Generating lex.yy.c"
    )
ENDIF ()

add_executable(ToyCompiler Source/main.cpp Header/lexer.h Header/Util/token.h Source/lexer.cpp
        Source/Util/Token.cpp Header/Util/symbols.h Source/Util/symbols.cpp Header/parser.h Header/Util/AstNode.h Source/parser.cpp Source/Util/AstNode.cpp)
include_directories(Header)

