cmake_minimum_required (VERSION 2.8)
project(SOIL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#file(GLOB SOURCE_FILES *.h *.c original/*.c)
file(GLOB SOURCE_FILES *.h *.c )

add_library(SOIL ${SOURCE_FILES})
