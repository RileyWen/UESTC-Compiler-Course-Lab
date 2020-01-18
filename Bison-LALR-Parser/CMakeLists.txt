cmake_minimum_required(VERSION 3.12)
project(Bison-LALR-Parser)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} headers)

BISON_TARGET(Parser sources/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(Scanner sources/scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_executable(Bison-LALR-Parser
        sources/Bison-LALR-Parser.cpp sources/AstNode.cpp sources/driver.cpp
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS})
