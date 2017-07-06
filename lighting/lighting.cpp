#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
#include <iostream>

float vertices_buffer[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

Camera camera(glm::vec3(0, 0, 3));

float timeDelta = 0;

double lastXpos = 0, lastYpos = 0;
double deltaXpos = 0, deltaYpos = 0;

void mouseCallBack(GLFWwindow* window, double xPos, double yPos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		deltaXpos = xPos - lastXpos;
		deltaYpos = lastYpos - yPos;
		lastXpos = xPos;
		lastYpos = yPos;

		camera.DirectionMove(deltaXpos, deltaYpos);
	}
	else {
		deltaXpos = 0;
		deltaYpos = 0;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.Zoom(yoffset);
}

void processInput(GLFWwindow* window) {
	const float speed = 20;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Move(CameraDirection::FORWARD, timeDelta);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Move(CameraDirection::BACKWARD, timeDelta);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Move(CameraDirection::LEFT, timeDelta);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Move(CameraDirection::RIGHT, timeDelta);
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		camera = Camera(glm::vec3(0, 0, 10), 0.f, -90.f);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwGetCursorPos(window, &lastXpos, &lastYpos);
	}
}
/*
const char* lampVertexShader = R"DLIM(#version 330 core
layout (location=0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1);
}

)DLIM";
*/
const char* lampVertexShader = R"DLIM(#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

)DLIM";
const char* lampFragmentShader = R"DLIM(#version 330 core

out vec4 FragColor;

void main() {
	FragColor = vec4(1);
}

)DLIM";

const char* objVertexShader = R"DLIM(#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 fragNorm;

void main() {
	fragPos = vec3(model * vec4(aPos, 1));
	fragNorm = mat3(transpose(inverse(model))) * aNorm;
	gl_Position = projection * view * vec4(fragPos, 1);
}

)DLIM";
//
const char* objFragShader = R"DLIM(#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 objColor;
uniform vec3 eyePos;

in vec3 fragPos;
in vec3 fragNorm;

void main() {
	float ambient_strength = 1;
	float diffuse_strength = 1;
	float specular_strength = 1;

	vec3 ambient = ambient_strength * light.ambient;

	vec3 lightDir = normalize(light.position - fragPos);
	vec3 norm = normalize(fragNorm);
	vec3 diffuse = diffuse_strength * light.diffuse * max(dot(norm, lightDir), 0) * material.diffuse;

	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	vec3 viewDir = normalize(eyePos - fragPos);
	float spec = dot(reflectDir, viewDir);
	vec3 specular = specular_strength * pow(max(spec, 0), material.shininess) * light.specular * material.specular;

	vec3 result = (ambient + diffuse + specular) * objColor;
	FragColor = vec4(result, 1);
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

	glfwSetCursorPosCallback(window, mouseCallBack);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, width, height);
	std::cout << "Width: " << width << ", Height:" << height << std::endl;

	Shader lampShader;
	lampShader.InitProgram(lampVertexShader, lampFragmentShader);
	Shader objShader;
	objShader.InitProgram(objVertexShader, objFragShader);

	GLuint cubeVao, vbo;
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_buffer), vertices_buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint lampVao;
	glGenVertexArrays(1, &lampVao);
	glBindVertexArray(lampVao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	float lastTime = (float)glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float time = (float)glfwGetTime();
		timeDelta = time - lastTime;
		lastTime = time;
		processInput(window);

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0, 0, 0));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.zoom_), float(width) / float(height), 0.1f, 100.f);
		
		objShader.Use();

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

		objShader.SetMatrix4f("model", glm::value_ptr(model));
		objShader.SetMatrix4f("view", glm::value_ptr(view));
		objShader.SetMatrix4f("projection", glm::value_ptr(projection));
		objShader.SetVec3f("light.position", glm::value_ptr(lightPos));
		objShader.SetVec3f("light.ambient", glm::value_ptr(ambientColor));
		objShader.SetVec3f("light.diffuse", glm::value_ptr(diffuseColor));
		objShader.SetVec3f("light.specular", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
		objShader.SetVec3f("objColor", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		objShader.SetVec3f("eyePos", glm::value_ptr(camera.pos_));
		objShader.SetVec3f("material.ambient", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		objShader.SetVec3f("material.diffuse", glm::value_ptr(glm::vec3(1.f, 0.5f, 0.31f)));
		objShader.SetVec3f("material.specular", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
		objShader.SetFloat("material.shininess", 32.f);

		glBindVertexArray(cubeVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.Use();
		lampShader.SetMatrix4f("model", glm::value_ptr(model));
		lampShader.SetMatrix4f("view", glm::value_ptr(view));
		lampShader.SetMatrix4f("projection", glm::value_ptr(projection));

		glBindVertexArray(lampVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &lampVao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}
