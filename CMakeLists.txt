cmake_minimum_required(VERSION 3.26)
project(Navigation)

set(CMAKE_CXX_STANDARD 23)

add_executable(Navigation main.cpp
        json.cpp
        json.h)
