set(SRC
	Camera.cpp
	Camera.h
	Shader.cpp
	Shader.h
)

add_library(common ${SRC})

target_link_libraries(common ${glew} ${glfw} ${opengl32})
