#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

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
	void ImguiSetup();
	void PrintHello();
private:
	int InitGlad();
	int InitGLFW();
	float testcolor[4];


	float MaxX = 10;
	float MaxY = 10;
	float Delta = 0.5;

	std::vector<float> testQueue;
};
