#include <glad/glad.h>
#include <iostream>
#include <glfw3.h>

using namespace std;

void frame_buffer_window_callback(GLFWwindow* window, int width, int height);
void init_glfw();
GLFWwindow* init_window_and_context();
void processInputs(GLFWwindow* window);

int main() {

	init_glfw();
	GLFWwindow* window = init_window_and_context();
	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}


	while (!glfwWindowShouldClose(window))
	{
		processInputs(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void frame_buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void init_glfw() {
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

}

GLFWwindow* init_window_and_context() {
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_window_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

void processInputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


