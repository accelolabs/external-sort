cmake_minimum_required(VERSION 3.10.0)
project(external-sort VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(external-sort main.cpp)
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE src/FileTape.cpp)
