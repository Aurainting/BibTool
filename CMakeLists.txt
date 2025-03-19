cmake_minimum_required(VERSION 3.30)
project(bibtool)

set(CMAKE_CXX_STANDARD 26)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/inc/")
