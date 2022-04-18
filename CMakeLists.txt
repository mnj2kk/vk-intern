cmake_minimum_required(VERSION 3.21)
project(vk-intern-update)

set(CMAKE_CXX_STANDARD 20)
include_directories("lib" "lib/my_test")
set(SRC_LIST
        source/parser.cpp lib/parser/parser.h
        source/suggestion.cpp lib/suggestion/suggestion.h
        lib/correcter/correcter.h source/correcter.cpp
        lib/query_generation/query_generation.h source/query_generation.cpp
        lib/solve/solve.h source/solve.cpp
        auto_test/auto_test.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
