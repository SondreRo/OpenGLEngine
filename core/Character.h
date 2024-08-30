#pragma once
#include <Actor.h>
#include <Mesh.h>

#include "MeshComponent.h"

class Character : public Actor {

	bool firstMouse;
	double lastX, lastY;

	bool UseCamera = false;

	public:

	Character();
	MeshComponent* CharacterMesh;
	void SetupCharacterMesh(Mesh* NewMesh);
	void processInput(struct GLFWwindow* window);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void Jump();
	bool onFloor = false;
	bool startJump = false;
	virtual void Update(glm::mat4 ParentMat) override;
	float CharacterSpeed = 30;
	float Gravity = 3;
	glm::vec3 Velocity = glm::vec3(0);
};
