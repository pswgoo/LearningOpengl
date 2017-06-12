#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
#include <iostream>

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

float vertices_buffer[] = {
	-0.5f, -0.5f, 0.f,  0.0f, 0.0f,
	0.5f, -0.5f, 0.f,  1.0f, 0.0f,
	0.5f,  0.5f, 0.f,  1.0f, 1.0f,
	0.5f,  0.5f, 0.f,  1.0f, 1.0f,
	-0.5f,  0.5f, 0.f,  0.0f, 1.0f,
	-0.5f, -0.5f, 0.f,  0.0f, 0.0f,

};

const char* kVertexAttribShader = R"DLIM(#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec2 texCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCoord = aTexCoord;
	color = vec4(0., 1. ,0. ,1.);
}

)DLIM";

const char* kFragmentAttribShader = R"DLIM(#version 330 core
out vec4 FragColor;
in vec4 color;
in vec2 texCoord;

uniform sampler2D texture0;

void main() {
	FragColor = texture(texture0, texCoord);
}

)DLIM";

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
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texture_data = stbi_load("../res/awesomeface.png", &img_width, &img_height, &img_channel, 0);
	std::cout << "iMG Width: " << img_width << ", iMG Height:" << img_height << std::endl;

	// Init texture 2D
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_buffer), vertices_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 6, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = (float)glfwGetTime();

		shader.Use();

		glm::mat4 translate;
		translate = glm::translate(translate, glm::vec3(0.5, 0.5, 0));
		glm::mat4 trans;
		trans = glm::rotate(trans, glm::radians(360.f * sin(time / 5)), glm::vec3(0.3, 0.5, 0.7));

		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0, 0, -3));

		glm::mat4 projection = glm::perspective(glm::radians(45.f), float(width) / float(height), 0.1f, 100.f);
/*
		shader.SetMatrix4f("model", glm::value_ptr(trans * translate));
		shader.SetMatrix4f("view", glm::value_ptr(view));
		shader.SetMatrix4f("projection", glm::value_ptr(projection));
*/
		shader.SetMatrix4f("model", glm::value_ptr(glm::mat4()));
		shader.SetMatrix4f("view", glm::value_ptr(glm::mat4()));
		shader.SetMatrix4f("projection", glm::value_ptr(glm::mat4()));

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
