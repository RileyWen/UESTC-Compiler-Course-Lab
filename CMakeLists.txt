cmake_minimum_required(VERSION 3.12)
project(ToyCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(ToyCompiler Source/main.cpp Header/lexer.h Header/Util/token.h Source/lexer.cpp)
include_directories(Header)