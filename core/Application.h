#pragma once
#include <Camera.h>
#include <vector>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <Actor.h>
#include <unordered_map>
#include <Character.h>

#include "CameraActor.h"

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

	
	std::unordered_map <std::string, Mesh*> mMeshes; 
	std::unordered_map <std::string, Actor*> mActors; 

	Camera defaultCamera;
	Character* character;
	CameraActor* camActor;
	class Window* window;

	//std::vector<Mesh*> mMeshes;
	ShaderProgram shaderProgram;
	Actor* GetActor(std::string ActorName);

	bool EnableDrawDebugBefore = false;
	bool EnableDrawDebugAfter = false;
private:
	Application() = default;
	~Application() = default;
	inline static Application* instance = nullptr;

	Mesh* GetMesh(std::string meshName);

	void SetupMeshes();
	void BindMeshes();
	void SetupActors();

	void SetupCallbacks();
	void processInput(class GLFWwindow* window);

	void Cleanup();
#define SOURCE_DIRECTORY(relativePath) std::string("../../../" + std::string(relativePath)) 
};

