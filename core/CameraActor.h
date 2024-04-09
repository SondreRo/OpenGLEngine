#pragma once
#include <Actor.h>

class ShaderProgram;

class CameraActor : public Actor {

public:
	virtual void Update(glm::mat4 ParentMat) override;

	glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));

	float yaw = -90;
	float pitch = 0;

	glm::vec3 LookAt;
	void RecalculateDirections();
	void RenderFromCamera(ShaderProgram shader_program);
	void AddRotation(float Rotation);
	float cameraDistance = 5;
};
