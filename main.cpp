#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mob.h"
#include "shaderClass.h"
#include "camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 calculateRayDirection(float mouseX, float mouseY, int screenWidth, int screenHeight,
	const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
bool intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
	const glm::vec3& sphereCenter, float sphereRadius);


bool shot = 0;
// ���������
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;

// ��������
float deltaTime = 0.0f;	// ����� ����� ������� ������ � ��������� ������
float lastFrame = 0.0f;

int main()
{
	// glfw: ������������� � ����������������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw: �������� ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// �������� GLFW, ����� �� �������� ��� ������
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: �������� ���� ���������� �� OpenGL-�������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ���������������� ����������� ��������� OpenGL
	glEnable(GL_DEPTH_TEST);

	// �������������� ����� ��������� ���������
	Shader ourShader("vertecs.shader", "fragment.shader");

	// �������� ������ (� ������(��)) � ��������� ��������� ���������
	float vertices[] = {
		// ����������        // ���������� ����������
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

	// ������� ���������� ����� �������
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	Mob armMob[10] = {
		Mob(vertices,cubePositions[0]),
		Mob(vertices,cubePositions[1]),
		Mob(vertices,cubePositions[2]),
		Mob(vertices,cubePositions[3]),
		Mob(vertices,cubePositions[4]),
		Mob(vertices,cubePositions[5]),
		Mob(vertices,cubePositions[6]),
		Mob(vertices,cubePositions[7]),
		Mob(vertices,cubePositions[8]),
		Mob(vertices,cubePositions[9])
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ������������ ��������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// �������� ���������� ���������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// �������� � �������� �������
	unsigned int texture1, texture2;

	// �������� �1 - ���������� ����
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// ��������� ���������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��������� ���������� ���������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �������� �����������, �������� �������� � ������������� ������-�������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // ��������� stb_image.h �� ��, ����� ����������� ��� ����������� �������� ��� y
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// �������� �2 - �������
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// ��������� ���������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ��������� ���������� ���������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// �������� �����������, �������� �������� � ������������� ������-�������
	data = stbi_load("shot.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// ���� awesomeface.png ����� �����-����� (������������), ������� ���������� ������������ �������� GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// ��������� OpenGL, ����� ������� � ������ ����������� ����� ����������� (��� ����� ������� ��������) 
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// ���� ����������
	while (!glfwWindowShouldClose(window))
	{
		// ���������� ����� ������ �� �������� ��� ������� �����
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ��������� �����
		processInput(window);

		// ���������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� ����� � ����� �������

		// �������� ������� � ��������������� ���������� ������
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// ���������� ������
		ourShader.use();

		// �������� ������� ������� �������� (��������� ������������ ������� ����� ��������, �� ��� ������������� ������ ��� ��� ������� �����)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// ������� �������������� ������/����
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);

		// �������� �����
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			if(!armMob[i].getUse()) {
				continue;
			}
			// ��������� ������� ������ ��� ������� ������� � �������� � � ������ �� ���������
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, armMob[i].getPosition());
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);

			if (shot) {

				glm::vec3 rayOrigin = camera.Position; // ������� ������
				glm::vec3 rayDirection = calculateRayDirection(lastX, lastY, width, height, projection, view);

				// ��������� �������
				glm::vec3 sphereCenter = armMob[i].getPosition(); // ����� �������
				float sphereRadius = 1.5f; // ������ �������
				// �������� �� �����������
				if (intersectRaySphere(rayOrigin, rayDirection, sphereCenter, sphereRadius)) {
					armMob[i].kill();
					continue;
				}
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		shot = 0;
		// glfw: ����� ���������� front- � back- �������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: ����������, ������������ ���� ���������� ����� GLFW-��c�����
	glfwTerminate();
	return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		shot = 1;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ����������, ��� ���� ��������� ������������� ����� �������� ����.
	// �������� ��������, ������ � ������ ����� ����������� ������, ��� �������, �� Retina-��������
	glViewport(0, 0, width, height);
}

// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // �����������, ��� ��� y-���������� ���� ����� �����

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: ������ ���, ����� �������������� �������� ����, ���������� ������ callback-�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

glm::vec3 calculateRayDirection(float mouseX, float mouseY, int screenWidth, int screenHeight,
	const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
	// 1. ����������� ���������� ���� � ��������������� ���������� ���������� (NDC)
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	float z = 1.0f;  // ��������� ����������� ����� ��� Z
	glm::vec3 rayNDC(x, y, z);

	// 2. ����������� ���������� � ������������ ���������
	glm::vec4 rayClip(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

	// 3. ����������� ���������� � ������������ ������
	glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	// 4. ����������� ���������� � ������� ������������
	glm::vec3 rayWorld = glm::vec3(glm::inverse(viewMatrix) * rayEye);
	rayWorld = glm::normalize(rayWorld); // ����������� ������ �����������

	return rayWorld;
}

bool intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
	const glm::vec3& sphereCenter, float sphereRadius) {
	glm::vec3 oc = rayOrigin - sphereCenter;
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(oc, rayDirection);
	float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0); // ���� ������������ �����������, �� ����������� ����
}