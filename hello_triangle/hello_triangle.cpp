#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb/stb_image.h"

#include <cmath>
#include <iostream>

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

float vertices[] = {
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
	0.5, 0.5, 0.0,
	-0.5, 0.5, 0
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

float vertices_work0[] = {
	-0.25, -0.25, 0.0,
	0.25, -0.25, 0.0,
	0.0, 0.25, 0.0,
	0.25, -0.25, 0.0,
	.75, -0.25, 0.0,
	.5, 0.25, 0.0,
};

float vertices_color[] = {
	// positions         // colors
	0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.f,1.f, // top right
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.f,0.f, // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.f,0.f, // bottom left
	-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,	0.f,1.f, // top left
};

// outColor = vec4(0f, 0.5f, 0f, 1.0f);
const char* kVertexShader = R"DLIM(#version 330 core
layout (location=0) in vec3 aPos;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

)DLIM";

const char* kFragmentShader = R"DLIM(#version 330 core
out vec4 FragColor;
uniform vec4 outColor;

void main() {
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = outColor;
}

)DLIM";

const char* kVertexAttribShader = R"DLIM(#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;

uniform float hOffset;

out vec4 outColor;
out vec2 outTexCoord;

void main() {
	gl_Position = vec4(aPos.x + hOffset, aPos.y, aPos.z, 1.0);
	outColor = vec4(aColor, 1.0f);
	outTexCoord = aTexCoord;
}

)DLIM";

const char* kFragmentAttribShader = R"DLIM(#version 330 core
out vec4 FragColor;
in vec4 outColor;
in vec2 outTexCoord;

uniform sampler2D ourTexture;

void main() {
	FragColor = texture(ourTexture, outTexCoord);
}

)DLIM";
//FragColor = texture(ourTexture, outTexCoord);
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOPENGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window";
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW init failed";
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	std::cout << "Width: " << width << ", Height:" << height << std::endl;

	int img_width, img_height, img_channel;
	unsigned char *texture_data = stbi_load("../res/container.jpg", &img_width, &img_height, &img_channel, 0);
	std::cout << "iMG Width: " << img_width << ", iMG Height:" << img_height << std::endl;

	// Init texture 2D
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);

	Shader shader;
	shader.InitProgram(kVertexAttribShader, kFragmentAttribShader);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unsigned int greenLocation = glGetUniformLocation(shaderProgram, "outColor");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = (float)glfwGetTime();
		float greenValue = sin(time) / 2.0f + 0.5f;

		//DrawTriangle();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		shader.Use();

		shader.SetFloat("hOffset", greenValue);

		//glUniform4f(greenLocation, .0f, greenValue, .0f, .0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
