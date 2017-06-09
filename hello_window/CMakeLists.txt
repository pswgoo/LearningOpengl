set(src hello_window.cpp)

add_executable(hello_window ${src})

target_link_libraries(hello_window ${glew} ${glfw} ${opengl32})
