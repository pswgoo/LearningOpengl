set(src hello_cube.cpp)

add_executable(hello_cube ${src})

target_link_libraries(hello_cube
	common
	stb
)
