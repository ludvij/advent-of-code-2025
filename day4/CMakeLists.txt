
cmake_minimum_required(VERSION 3.31)

project(2025_04)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}_01)
add_executable(${PROJECT_NAME}_02)

target_sources(${PROJECT_NAME}_01 PRIVATE src/part1.cpp)
target_sources(${PROJECT_NAME}_02 PRIVATE src/part2.cpp)

target_link_libraries(${PROJECT_NAME}_01 PRIVATE ludutils)
target_link_libraries(${PROJECT_NAME}_02 PRIVATE ludutils)
