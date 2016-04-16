cmake_minimum_required(VERSION 2.8)
project(lec_acc_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(SOURCE_FILES
        Main.cpp
        Target.h
        Neighbours.cpp
        Neighbours.h
        Highway.cpp
        Highway.h
        Vehicle.cpp
        Vehicle.h
        Window2D.cpp
        Window2D.h
        Lane.cpp
        Lane.h
        Window.cpp
        Window.h
        Error.h
        Interval.h
        RandomVehicle.cpp
        RandomVehicle.h
        Foliage2D.cpp Foliage2D.h
        ACCVehicle.cpp ACCVehicle.h)
add_executable(lec_acc_cpp ${SOURCE_FILES})


# We use OpenGL as a backend for drawing stuff
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(lec_acc_cpp ${OPENGL_LIBRARIES})


add_subdirectory(glfw)
include_directories(glfw/include)
target_link_libraries(lec_acc_cpp ${GLFW_LIBRARIES})

add_subdirectory(imgui)
include_directories(imgui)
target_link_libraries(lec_acc_cpp imgui)


#add_subdirectory(CEGUI)
#include_directories(CEGUI/cegui/include ${CEGUI_INCLUDE_DIRS})
#target_link_libraries(lec_acc_cpp ${CEGUI_LIBRARIES})

# We'll load these with PkgConfig
#find_package(PkgConfig REQUIRED)

# This is for a window and some key events
#pkg_search_module(GLFW REQUIRED glfw3)
#include_directories(${GLFW_INCLUDE_DIRS})
#target_link_libraries(lec_acc_cpp ${GLFW_LIBRARIES})
#target_link_libraries(lec_acc_cpp ${GLFW_STATIC_LIBRARIES})
