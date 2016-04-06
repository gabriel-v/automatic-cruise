cmake_minimum_required(VERSION 3.3)
project(lec_acc_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
        Main.cpp
        Highway.cpp
        Highway.h
        Vehicle.cpp
        Vehicle.h
        Simulation.cpp
        Simulation.h
        Lane.cpp
        Lane.h
        UserInterface.cpp
        UserInterface.h)
add_executable(lec_acc_cpp ${SOURCE_FILES})