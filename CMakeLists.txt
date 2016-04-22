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
        Foliage2D.cpp
        Foliage2D.h
        ACCVehicle.cpp
        ACCVehicle.h
        imgui_impl_glfw.cpp
        imgui_impl_glfw.h
		UIPresenter.cpp UIPresenter.h)

add_executable(lec_acc_cpp ${SOURCE_FILES})


# We use OpenGL as a backend for drawing stuff
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(lec_acc_cpp ${OPENGL_LIBRARIES})

# Load GLFW3
IF(APPLE)
	# For Stefan's $1k toy
	add_subdirectory(glfw)
	include_directories(glfw/include)
	target_link_libraries(lec_acc_cpp ${GLFW_LIBRARIES})
	target_link_libraries(lec_acc_cpp ${GLFW_STATIC_LIBRARIES})
ELSE()
	# We'll load these with PkgConfig
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	target_link_libraries(lec_acc_cpp ${GLFW_LIBRARIES})
	target_link_libraries(lec_acc_cpp ${GLFW_STATIC_LIBRARIES})
ENDIF()

add_subdirectory(imgui)
include_directories(imgui)
target_link_libraries(lec_acc_cpp imgui)

