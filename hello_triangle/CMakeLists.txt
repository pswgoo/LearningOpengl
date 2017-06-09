set(src hello_triangle.cpp)

add_executable(hello_triangle ${src})

target_link_libraries(hello_triangle ${glew} ${glfw} ${opengl32})
