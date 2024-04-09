#include <iostream>
#include <Window.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

int Window::CreateWindow()
{
	InitGLFW();

	mWindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	Width = 800;
	Height = 600;
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);
	InitGlad();

	glViewport(0, 0, 800, 600);

	return 1;
}

GLFWwindow* Window::GetGLFWwindow()
{
	return mWindow;
}

int Window::InitGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 1;
}

int Window::InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	return 1;
}
