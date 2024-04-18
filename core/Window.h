#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui/imconfig.h>
#include <vector>
#include <Mesh.h>

#include "Actor.h"

//#include <glad/glad.h>
//#include "GLFW/glfw3.h"

class Window {
public:
	Window() = default;
	int CreatemyWindow();


	//GLFWwindow* GetMyGLFWwindow();
	GLFWwindow* mWindow;

	int Height;
	int Width;

	void StartFrame();
	void EndFrame();

	// Callbacks
	void WindowFramebufferSizeCallback(int width, int height);
	void WindowMouseMoveCallback(double xpos, double ypos);
	void WindowMouseButtonCallback(int button, int action, int mods);
	void WindowMouseScrollCallback(double xoffset, double yoffset);
	void WindowKeyCallback(int key, int scancode, int action, int mods);
	void WindowCharCallback(GLFWwindow* window, unsigned int c);

	void ImguiSetup();

	void Test();

	void LandScapeGenerator();

	void ImportMeshMenu();
	void MeshList();
	void ActorList();

	void PrintHello();
private:

	char DisplayName[100] = { 0 };
	char ImportPath[300] = { 0 };

	bool LockScale = false;

	int InitGlad();
	int InitGLFW();
	float testcolor[4];

	Mesh* SelectedMesh;
	Actor* SelectedActor;

	float MaxX = 40;
	float MaxY = 40;
	float Delta = 2;
	int Type = 1;

	std::vector<float> testQueue;
};
