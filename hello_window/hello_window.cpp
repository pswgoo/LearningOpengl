#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	std::cout << "Width: " << width << ", Height:" << height << std::endl;

	glfwSetKeyCallback(window, onKeyCallback);

	int cnt = 0;
	int loop = 10000;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		int tmp = cnt / loop;
		glClearColor((tmp + 1) % 3, tmp % 3, tmp % 2, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		++cnt;
	}

	glfwTerminate();
	return 0;
}
