project(LearningOpengl)

cmake_minimum_required(VERSION 3.0)

find_library(opengl32 opengl32.lib "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64")

find_library(glfw NAMES glfw3.lib PATHS lib)
find_library(glew NAMES glew32s.lib PATHS lib)

#if (MSVC)
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /NODEFAULTLIB:libcmt.lib)
#endif()
	
include_directories(include common external ./)

add_subdirectory(common)
add_subdirectory(external)

add_subdirectory(hello_window)
add_subdirectory(hello_triangle)

file(COPY res DESTINATION ${CMAKE_BINARY_DIR})
