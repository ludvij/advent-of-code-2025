cmake_minimum_required(VERSION 3.31)

project(2025_12)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}_01 src/part1.cpp)

target_link_libraries(${PROJECT_NAME}_01 PRIVATE ludutils)
