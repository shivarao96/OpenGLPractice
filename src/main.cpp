#include <glad/glad.h>
#include <iostream>
#include <glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "TextureHandler.h"
#include "Mesh.h"

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
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollBack(GLFWwindow* window, double xOffset, double yOffset);
Mesh::MeshConfig* drawCube(std::vector<Mesh::TextureInfo> textures);
Mesh::MeshConfig* drawPlane(std::vector<Mesh::TextureInfo> textures);

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

	//..cube mesh
	Shader cubeShader("./shaders/Model.vert", "./shaders/Model.frag");
	std::vector<Mesh::TextureInfo> cubeTextures;
	Mesh::TextureInfo cubeTexture1;
	cubeTexture1.texture = new TextureHandler("./assets/textures/marble.jpg", false);
	cubeTexture1.type = "texture_diffuse";
	cubeTextures.push_back(cubeTexture1);
	Mesh::MeshConfig* cubeMesh = drawCube(cubeTextures);

	//..Plane mesh
	Shader planeShader("./shaders/Model.vert", "./shaders/Model.frag");

	std::vector<Mesh::TextureInfo> planeTextures;
	Mesh::TextureInfo planeTexture1;
	planeTexture1.texture = new TextureHandler("./assets/textures/metal.png", false);
	planeTexture1.type = "texture_diffuse";
	planeTextures.push_back(planeTexture1);
	Mesh::MeshConfig* planeMesh = drawPlane(planeTextures);

	//..grass cube mesh
	std::vector<Mesh::TextureInfo> grassTextures;
	Mesh::TextureInfo grassTexture1;
	grassTexture1.texture = new TextureHandler("./assets/textures/grass.png", false);
	grassTexture1.type = "texture_diffuse";
	grassTextures.push_back(grassTexture1);
	Mesh::MeshConfig* grassCubeMesh = drawCube(grassTextures);
	Shader outlineShader("./shaders/Model.vert", "./shaders/Model.frag");


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputs();
		glClearColor(1.0f, 0.5f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		glm::mat4 commonView = newCam.getViewMatrix();
		glm::mat4 commonProjection = glm::perspective(glm::radians(newCam.getZoomVal()), screenWidth / screenHeight, 0.1f, 100.0f);

		//..render plane
		glStencilMask(0x00);
		planeShader.use();
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::translate(planeModel, glm::vec3(0, -0.001, 0));
		planeShader.setMat4("model", planeModel);
		planeShader.setMat4("view", commonView);
		planeShader.setMat4("projection", commonProjection);
		planeMesh->drawMesh(planeShader);

		//..render cube
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		cubeShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -2));
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", commonView);
		cubeShader.setMat4("projection", commonProjection);
		cubeMesh->drawMesh(cubeShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2, 0, 2));
		cubeShader.setMat4("model", model);
		cubeMesh->drawMesh(cubeShader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		//..render scaled cube
		outlineShader.use();
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//glm::mat4 model = glm::mat4(1.0f);
		float scale = 0.5;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -2));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		outlineShader.setMat4("model", model);
		outlineShader.setMat4("view", commonView);
		outlineShader.setMat4("projection", commonProjection);
		grassCubeMesh->drawMesh(outlineShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2, 0, 2));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		outlineShader.setMat4("model", model);
		grassCubeMesh->drawMesh(outlineShader);
		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete cubeMesh;
	for (unsigned int i = 0; i < cubeTextures.size(); i++) {
		delete cubeTextures[i].texture;
	}
	delete planeMesh;
	for (unsigned int i = 0; i < planeTextures.size(); i++) {
		delete planeTextures[i].texture;
	}
	delete grassCubeMesh;
	for (unsigned int i = 0; i < grassTextures.size(); i++) {
		delete grassTextures[i].texture;
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
Mesh::MeshConfig* drawCube(std::vector<Mesh::TextureInfo> textures) {
	float cubePosition[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	float cubeNormals[] = {
		
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,

		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};
	float cubeTextures[] = {
		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		0.0f,  0.0f,
		1.0f,  0.0f,
		1.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,

		1.0f,  0.0f,
		1.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,

		0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f,

		0.0f,  1.0f,
		1.0f,  1.0f,
		1.0f,  0.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f
	};
	std::vector<Mesh::VertexInfo> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < 36; i++) {
		Mesh::VertexInfo vTemp;
		int tempIndex = i * 3;
		int tempTextIndex = i * 2;
		vTemp.vertices = glm::vec3(
			cubePosition[tempIndex],
			cubePosition[tempIndex + 1],
			cubePosition[tempIndex + 2]
		);
		vTemp.normal = glm::vec3(
			cubeNormals[tempIndex],
			cubeNormals[tempIndex + 1],
			cubeNormals[tempIndex + 2]
		);
		vTemp.texCoords = glm::vec2(
			cubeTextures[tempTextIndex],
			cubeTextures[tempTextIndex + 1]
		);
		vTemp.tangent = glm::vec3(0);
		vTemp.bitTangent = glm::vec3(0);

		vertices.push_back(vTemp);
	}

	return new Mesh::MeshConfig(
		vertices,
		indices,
		textures
	);
}

Mesh::MeshConfig* drawPlane(std::vector<Mesh::TextureInfo> textures) {
	std::vector<Mesh::VertexInfo> vertices;
	std::vector<unsigned int> indices;

	float planePosition[] = {
		5.0f, -0.5f,  5.0f,
	   -5.0f, -0.5f,  5.0f,
	   -5.0f, -0.5f, -5.0f,

		5.0f, -0.5f,  5.0f,
	   -5.0f, -0.5f, -5.0f,
		5.0f, -0.5f, -5.0f
	};

	float planeTexture[] = {
		2.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 2.0f,

		2.0f, 0.0f,
		0.0f, 2.0f,
		2.0f, 2.0f
	};

	for (unsigned int i = 0; i < 6; i++) {
		Mesh::VertexInfo vTemp;
		int tempIndex = i * 3;
		int tempTextIndex = i * 2;
		vTemp.vertices = glm::vec3(
			planePosition[tempIndex],
			planePosition[tempIndex + 1],
			planePosition[tempIndex + 2]
		);
		vTemp.texCoords = glm::vec2(
			planeTexture[tempTextIndex],
			planeTexture[tempTextIndex + 1]
		);
		vTemp.normal = glm::vec3(0);
		vTemp.tangent = glm::vec3(0);
		vTemp.bitTangent = glm::vec3(0);
		vertices.push_back(vTemp);
	}

	return new Mesh::MeshConfig(
		vertices,
		indices,
		textures
	);
}