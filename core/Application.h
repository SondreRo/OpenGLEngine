#pragma once
#include <Camera.h>
#include <vector>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <Actor.h>
#include <unordered_map>
#include <Character.h>
#include <LineMesh.h>
#include "CameraActor.h"
#include "CurveMesh.h"
#include "Window.h"

class Application {
public:
	static Application& get();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	static void destruct()
	{
		delete instance;
		instance = nullptr;
	}

	std::vector <Actor*> actors;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	int init();

	int Run();


	LineMesh* line_mesh;
	CurveMesh* curve_mesh;

	std::unordered_map <std::string, Mesh*> mMeshes; 
	std::unordered_map <std::string, Actor*> mActors; 

	Camera defaultCamera;
	Character* character;
	CameraActor* camActor;
	Window* mWindow;

	//std::vector<Mesh*> mMeshes;
	ShaderProgram shaderProgram;
	ShaderProgram LineshaderProgram;
	Actor* GetActor(std::string ActorName);
	Mesh* GetMesh(std::string meshName);

	bool EnableDrawDebugBefore = false;
	bool EnableDrawDebugAfter = false;

	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	float AverageFrameRate;
	void RemakeLandscape(float maxX, float maxY, float delta);
private:
	Application() = default;
	~Application() = default;
	inline static Application* instance = nullptr;


	void SetupMeshes();
	void BindMeshes();
	void SetupActors();

	void SetupCallbacks();
	void processInput(class GLFWwindow* window);

	void Cleanup();
	std::vector<float> frames;
#define SOURCE_DIRECTORY(relativePath) std::string("../../../" + std::string(relativePath)) 
};

