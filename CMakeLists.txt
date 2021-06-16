cmake_minimum_required(VERSION 3.13)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CMakeCompileOptions.txt)

include_directories(logger)
add_subdirectory(logger)

include_directories(dispatcher)
add_subdirectory(dispatcher)

include_directories(math)
add_subdirectory(math)

include_directories(incubus)
add_subdirectory(incubus)
