#include <glad/glad.h>
#include <iostream>
#include <glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "TextureHandler.h"
#include "Model.h"

//varibale declaration
extern GLFWwindow* window = nullptr;
CameraObject::Camera newCam(
	glm::vec3(0.0f, 0.0f, 5.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
);

float screenWidth = 1280;
float screenHeight = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
bool isFirstMouseEvent = true;

glm::vec3 lightPos = glm::vec3(-1.2f, -1.0f, 2.0f);

// func prototype
void frame_buffer_window_callback(GLFWwindow* window, int width, int height);
void init_glfw();
bool init_window_and_context();
void processInputs();
void renderStuff();
//unsigned int getTextureId(const char* fileName);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollBack(GLFWwindow* window, double xOffset, double yOffset);

int main() {
	init_glfw();
	if (!init_window_and_context()) {
		glfwTerminate();
		return -1;
	}
	renderStuff();

	glfwTerminate();
	return 0;
}

// functtion declarations
void frame_buffer_window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void init_glfw() {
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}
bool init_window_and_context() {
	window = glfwCreateWindow(screenWidth, screenHeight, "My Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_window_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, mouseScrollBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	return true;
}
void processInputs() {
	static bool showWireFrame = true;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		showWireFrame = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		showWireFrame = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		newCam.processKeyboard(CameraObject::CameraMovement::DOWN, deltaTime);
	}
}
void renderStuff() {

	Shader modelShader("./shaders/Model.vert", "./shaders/Model.frag");
	Model exampleModel("./assets/objects/nanosuit/nanosuit.obj");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputs();
		//glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 commonView = newCam.getViewMatrix();
		glm::mat4 commonProjection = glm::perspective(glm::radians(newCam.getZoomVal()), screenWidth / screenHeight, 0.1f, 100.0f);;

		modelShader.use();

		modelShader.setMat4("view", commonView);
		modelShader.setMat4("projection", commonProjection);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelShader.setMat4("model", model);

		exampleModel.drawModel(modelShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (isFirstMouseEvent) {
		lastX = xPos;
		lastY = yPos;
		isFirstMouseEvent = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	newCam.processMouseMovement(xOffset, yOffset, true);
}
void mouseScrollBack(GLFWwindow* window, double xOffset, double yOffset) {
	newCam.processMouseScroll((float)yOffset);
}