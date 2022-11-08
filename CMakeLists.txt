cmake_minimum_required(VERSION 3.20)
project(ioc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)


