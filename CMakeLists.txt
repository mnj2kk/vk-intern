cmake_minimum_required(VERSION 3.21)
project(vk_intern)

set(CMAKE_CXX_STANDARD 20)

add_executable(vk_intern suggestion.h suggestion.cpp Test_library/main.cpp)
