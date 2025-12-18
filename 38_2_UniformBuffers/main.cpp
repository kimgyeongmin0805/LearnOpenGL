#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include <vector>
#include <algorithm>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
unsigned int loadCubeTexture(const std::string&, const std::vector<std::string>&);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// ------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// build and compile our shader program
	// ------------------------------------
	Shader shaderRed("object.vs", "red.fs");
	Shader shaderGreen("object.vs", "green.fs");
	Shader shaderBlue("object.vs", "blue.fs");
	Shader shaderYellow("object.vs", "yellow.fs");
	Shader screenShader("screen.vs", "screen.fs");

	// set up vertex data(and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
		// objectVertices를 position과 normal로 분리
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
	float quadVertices[] = { 
		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
	};

	// object VAO, VBO
	unsigned int objectVAO, objectVBO;
	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);
	glGenBuffers(1, &objectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// screen VAO, VBO
	unsigned int screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


	// configure a uniform buffer object
	// ----------------------------------
	// block index
	unsigned int indexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
	unsigned int indexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
	unsigned int indexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
	unsigned int indexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
	// uniform block binding
	glUniformBlockBinding(shaderRed.ID, indexRed, 0);
	glUniformBlockBinding(shaderGreen.ID, indexGreen, 0);
	glUniformBlockBinding(shaderBlue.ID, indexBlue, 0);
	glUniformBlockBinding(shaderYellow.ID, indexYellow, 0);
	// create ubo
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
	// initialize projection matrix and view matrix
	glm::mat4 projection = camera.GetProjectionMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// framebuffer configuration
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// create a color attachment texture
	unsigned int textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
	// create a renderbuffer object for depth and stencil attachment
	unsigned int RBO;
	glGenTextures(1, &RBO);
	glBindTexture(GL_TEXTURE_2D, RBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, RBO, 0);
	// complete check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// load textures
	screenShader.use();
	screenShader.setInt("screenTexture", 0);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {

		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// first render
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// object
		glm::mat4 view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(objectVAO);
		// red cube
		shaderRed.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
		shaderRed.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// green cube
		shaderGreen.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
		shaderGreen.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// blue cube
		shaderBlue.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
		shaderBlue.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// yellow cube
		shaderYellow.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
		shaderYellow.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// second render
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteBuffers(1, &objectVBO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenVBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow*, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}

// glfw: whenver the mouse scroll wheel scrolls, this callback is called
// ---------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// load a texture from file
// -------------------------
unsigned int loadTexture(const char* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (nrChannels == 1) {
			GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}
		else {
			GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}

		GLint wrap_param = (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR::STB_IMAGE::FAILED TO LOAD IMAGE AT PATH: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}