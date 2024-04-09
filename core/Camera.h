#pragma once
#include "glm/glm.hpp"


class ShaderProgram;
class GLFWwindow;

class Camera {

	bool firstMouse;
	double lastX, lastY;

	bool UseCamera = false;

public:

	glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));

	float yaw = -90;
	float pitch = 0;

	void RecalculateDirections();
	void RenderFromCamera(ShaderProgram shader_program);
	void processInput(GLFWwindow* window);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};
