cmake_minimum_required(VERSION 2.8)
project(tic_tac_toe_rl)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_CXX_FLAGS "-g -Wall -Wextra -O3")

include_directories(include)
set(CMAKE_SOURCE_DIR src/)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})