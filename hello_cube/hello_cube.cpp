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
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCoord = aTexCoord;
	color = vec4(0., 1. ,0. ,1.);
}

)DLIM";

const char* kFragmentAttribShader = R"DLIM(#version 330 core
out vec4 FragColor;
in vec4 color;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), .2);
}

)DLIM";

glm::vec3 cameraPos(0, 0, 10);
glm::vec3 cameraDir(0, 0, -1);
glm::vec3 cameraUp(0, 1, 0);

float timeDelta = 0;

double lastXpos = 0, lastYpos = 0;
double deltaXpos = 0, deltaYpos = 0;
float picth = 0, yaw = -90;

void mouseCallBack(GLFWwindow* window, double xPos, double yPos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		deltaXpos = xPos - lastXpos;
		deltaYpos = lastYpos - yPos;
		lastXpos = xPos;
		lastYpos = yPos;

		const float sensitivity = 0.1;
		picth += deltaYpos * sensitivity;
		yaw += deltaXpos * sensitivity;

		if (picth > 89)
			picth = 89;
		if (picth < -89)
			picth = -89;

		glm::vec3 dir;
		cameraDir.y = sin(glm::radians(picth));
		cameraDir.x = cos(glm::radians(picth)) * cos(glm::radians(yaw));
		cameraDir.z = cos(glm::radians(picth)) * sin(glm::radians(yaw));

		dir.x += deltaXpos * sensitivity;
		dir.y += deltaYpos * sensitivity;

		//cameraPos += dir;
		std::cout << "Camera: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
		std::cout << "dir: " << dir.x << " " << dir.y << " " << dir.z << std::endl;
		std::cout << "picth: " << picth << ", yaw: " << yaw << std::endl;
	}
	else {
		deltaXpos = 0;
		deltaYpos = 0;
	}
}

void processInput(GLFWwindow* window) {
	const float speed = 20;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraDir * timeDelta * speed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraDir * timeDelta * speed;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * timeDelta * speed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * timeDelta * speed;
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		cameraPos = glm::vec3(0, 0, 10);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &lastXpos, &lastYpos);
	}
}

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

	glfwSetCursorPosCallback(window, mouseCallBack);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	std::cout << "Width: " << width << ", Height:" << height << std::endl;

	int img_width, img_height, img_channel;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char *texture_data = stbi_load("../res/container.jpg", &img_width, &img_height, &img_channel, 0);
	std::cout << "iMG Width: " << img_width << ", iMG Height:" << img_height << std::endl;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texture_data1 = stbi_load("../res/awesomeface.png", &img_width, &img_height, &img_channel, 0);
	std::cout << "iMG Width: " << img_width << ", iMG Height:" << img_height << std::endl;

	// Init texture 2D
	GLuint texture0;
	glGenTextures(1, &texture0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data1);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data1);

	Shader shader;
	shader.InitProgram(kVertexAttribShader, kFragmentAttribShader);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_buffer), vertices_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::vec3 translations[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -4.f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -4.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float lastTime = (float)glfwGetTime();
	int frame_cnt = 0;
	while (!glfwWindowShouldClose(window)) {
		float time = (float)glfwGetTime();
		timeDelta = time - lastTime;
		lastTime = time;
		processInput(window);


		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
*/
		shader.Use();
		shader.SetInt("texture0", 0);
		shader.SetInt("texture1", 1);

		glm::mat4 view;

		view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		glm::mat4 projection = glm::perspective(glm::radians(45.f), float(width) / float(height), 0.1f, 100.f);

		shader.SetMatrix4f("view", glm::value_ptr(view));
		shader.SetMatrix4f("projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);

		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(0.5, 0.5, 0.5));

		glm::mat4 trans;
		trans = glm::rotate(trans, glm::radians(360.f * sin(time / 5)), glm::vec3(0.3, 0.5, 0.7));
		for (int i = 0; i < 10; ++i) {
			glm::mat4 translate;
			translate = glm::translate(translate, translations[i]);
			shader.SetMatrix4f("model", glm::value_ptr(translate * trans * scale));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
