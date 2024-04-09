#pragma once

class GLFWwindow;

class Window {
public:

	int CreateWindow();

	GLFWwindow* GetGLFWwindow();

	int Height;
	int Width;
private:
	int InitGlad();
	int InitGLFW();

	
	GLFWwindow* mWindow;
};
