cmake_minimum_required(VERSION 3.10)

project(math)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    shape.cpp)

set(HDRS
    real_number.h
    figure.h
    shape.h
    coordinates.h
    color.h)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HDRS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
include_directories(${PROJECT_NAME})
