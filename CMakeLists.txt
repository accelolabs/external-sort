cmake_minimum_required(VERSION 3.10.0)
project(external-sort VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(external_sort_lib
    src/FileTape.cpp
)

add_executable(external-sort src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE external_sort_lib)

# file(COPY ${CMAKE_SOURCE_DIR}/example/ DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()
add_subdirectory(test)
