#include <glad/glad.h>
#include <iostream>
#include <glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "TextureHandler.h"

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
	Shader containerShader("./shaders/container.vert", "./shaders/container.frag");
	Shader lightShader("./shaders/light.vert", "./shaders/light.frag");
	
	TextureHandler texture("./assets/textures/grass.png", false);
	TextureHandler wall("./assets/textures/wall.jpg", false);
	TextureHandler face("./assets/textures/awesomeface.png", true);
	TextureHandler woodenContainer("./assets/textures/container2.png", false);
	TextureHandler woodenContainerBorder("./assets/textures/container2_specular.png", false);

	float vertices2[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3 
	};

	unsigned int VAO2, VBO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputs();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 commonView = newCam.getViewMatrix();
		glm::mat4 commonProjection = glm::perspective(glm::radians(newCam.getZoomVal()), screenWidth / screenHeight, 0.1f, 100.0f);;

		//..::light container::..//
		lightShader.use();
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.25f));
		lightModel = glm::rotate(lightModel, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.2f, 1.0f, 0.3f));
		lightShader.setMat4("model", lightModel);
		lightShader.setMat4("view", commonView);
		lightShader.setMat4("projection", commonProjection);
		lightShader.setVec3("lightColor", glm::vec3(1,1,1));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//..::Container handling::..//
		containerShader.use();
		containerShader.setVec3("lightPos", lightPos);
		containerShader.setVec3("light.position", newCam.getPosition());
		containerShader.setVec3("light.direction", newCam.getFront());
		containerShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		containerShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		containerShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		containerShader.setFloat("light.constant", 1.0f);
		containerShader.setFloat("light.linear", 0.09f);
		containerShader.setFloat("light.quadratic", 0.032f);
		containerShader.setFloat("light.cuttOff", glm::cos(glm::radians(12.5f)));
		containerShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5)));

		//..::Container Direction light::..//
		containerShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		containerShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		containerShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		containerShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//..::Container Point light::..//
		containerShader.setVec3("pointLights.position", lightPos);
		containerShader.setVec3("pointLights.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		containerShader.setVec3("pointLights.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		containerShader.setVec3("pointLights.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		containerShader.setFloat("pointLights.constant", 1.0f);
		containerShader.setFloat("pointLights.linear", 0.09);
		containerShader.setFloat("pointLights.quadratic", 0.032);

		containerShader.setVec3("sptLight.position", newCam.getPosition());
		containerShader.setVec3("sptLight.direction", newCam.getFront());
		containerShader.setVec3("sptLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		containerShader.setVec3("sptLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		containerShader.setVec3("sptLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		containerShader.setFloat("sptLight.constant", 1.0f);
		containerShader.setFloat("sptLight.linear", 0.09);
		containerShader.setFloat("sptLight.quadratic", 0.032);
		containerShader.setFloat("sptLight.cuttOff", glm::cos(glm::radians(12.5f)));
		containerShader.setFloat("sptLight.outerCutOff", glm::cos(glm::radians(17.5)));


		containerShader.setVec3("viewPos", newCam.getPosition());
		containerShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		containerShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		containerShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		containerShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		containerShader.setFloat("material.shininess", 32.0f);
		containerShader.setInt("material.diffuse", 0);
		containerShader.setInt("material.specular", 1);

		glm::mat4 containerModel = glm::mat4(1.0f);
		containerShader.setMat4("model", containerModel);
		containerShader.setMat4("view", commonView);
		containerShader.setMat4("projection", commonProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodenContainer.getTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, woodenContainerBorder.getTextureId());
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
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