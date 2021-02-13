#include <glad/glad.h>
#include <iostream>
#include <glfw3.h>
#include <map>
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
Mesh::MeshConfig* drawGrass(std::vector<Mesh::TextureInfo> textures);

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
	/*Shader cubeShader("./shaders/Model.vert", "./shaders/Model.frag");
	std::vector<Mesh::TextureInfo> cubeTextures;
	Mesh::TextureInfo cubeTexture1;
	cubeTexture1.texture = new Texture::TextureHandler("./assets/textures/grass-cube.png", false);
	cubeTexture1.type = "texture_diffuse";
	cubeTextures.push_back(cubeTexture1);
	Mesh::MeshConfig* cubeMesh = drawCube(cubeTextures);*/

	Shader shaderRed("./shaders/advanced_glsl.vert", "./shaders/red_advance_glsl.frag");
	Shader shaderBlue("./shaders/advanced_glsl.vert", "./shaders/blue_advance_glsl.frag");
	Shader shaderGreen("./shaders/advanced_glsl.vert", "./shaders/green_advance_glsl.frag");
	Shader shaderYellow("./shaders/advanced_glsl.vert", "./shaders/yellow_advance_glsl.frag");

	float cubeVertices[] = {
		// positions         
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
		-0.5f,  0.5f, -0.5f,
	};
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.getShaderId(), "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.getShaderId(), "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.getShaderId(), "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.getShaderId(), "Matrices");

	glUniformBlockBinding(shaderRed.getShaderId(), uniformBlockIndexRed, 0);
	glUniformBlockBinding(shaderBlue.getShaderId(), uniformBlockIndexBlue, 0);
	glUniformBlockBinding(shaderGreen.getShaderId(), uniformBlockIndexGreen, 0);
	glUniformBlockBinding(shaderYellow.getShaderId(), uniformBlockIndexYellow, 0);

	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(
		GL_UNIFORM_BUFFER, 
		2 * sizeof(glm::mat4), 
		NULL, 
		GL_STATIC_DRAW
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(
		GL_UNIFORM_BUFFER,
		0,
		uboMatrices,
		0,
		2 * sizeof(glm::mat4)
	);

	glm::mat4 commonProjection = glm::perspective(
		glm::radians(newCam.getZoomVal()),
		screenWidth / screenHeight,
		0.1f,
		100.0f
	);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(
		GL_UNIFORM_BUFFER,
		0,
		sizeof(glm::mat4),
		glm::value_ptr(commonProjection)
	);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInputs();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 commonView = newCam.getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(
			GL_UNIFORM_BUFFER,
			sizeof(glm::mat4),
			sizeof(glm::mat4),
			glm::value_ptr(commonView)
		);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
				

		/*cubeShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		cubeShader.setMat4("view", commonView);
		cubeShader.setMat4("projection", commonProjection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, 0));
		cubeShader.setMat4("model", model);
		cubeMesh->drawMesh(cubeShader);*/

		glBindVertexArray(cubeVAO);
		shaderRed.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
		shaderRed.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// GREEN
		shaderGreen.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
		shaderGreen.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// YELLOW
		shaderYellow.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
		shaderYellow.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// BLUE
		shaderBlue.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
		shaderBlue.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*delete cubeMesh;
	for (unsigned int i = 0; i < cubeTextures.size(); i++) {
		delete cubeTextures[i].texture;
	}*/
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
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	// Front face
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	// Left face
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// Right face
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 // Bottom face
	 -0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f, -0.5f,
	  0.5f, -0.5f,  0.5f,
	  0.5f, -0.5f,  0.5f,
	 -0.5f, -0.5f,  0.5f,
	 -0.5f, -0.5f, -0.5f,
	 // Top face
	 -0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f,  0.5f,
	  0.5f,  0.5f, -0.5f,
	  0.5f,  0.5f,  0.5f,
	 -0.5f,  0.5f, -0.5f,
	 -0.5f,  0.5f,  0.5f
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
		// Back face
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		// Front face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		// Left face
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		// Right face
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		// Bottom face
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		// Top face
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
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
Mesh::MeshConfig* drawGrass(std::vector<Mesh::TextureInfo> textures) {
	std::vector<Mesh::VertexInfo> vertices;
	std::vector<unsigned int> indices;
	float grassVertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	for (unsigned int i = 0; i < 6; i++) {
		Mesh::VertexInfo vTemp;
		int tempIndex = i * 5;
		vTemp.vertices = glm::vec3(
			grassVertices[tempIndex],
			grassVertices[tempIndex + 1],
			grassVertices[tempIndex + 2]
		);
		vTemp.texCoords = glm::vec2(
			grassVertices[tempIndex + 3],
			grassVertices[tempIndex + 4]
		);
		vTemp.normal = glm::vec3(0.0f);
		vTemp.tangent = glm::vec3(0.0f);
		vTemp.bitTangent = glm::vec3(0.0f);;

		vertices.push_back(vTemp);
	}

	return new Mesh::MeshConfig(
		vertices,
		indices,
		textures
	);
}