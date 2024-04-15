#include <Window.h>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include <iostream>

#include "Application.h"
#include "Enemy.h"
#include "glm/vec3.hpp"

int Window::CreatemyWindow()
{
	InitGLFW();
	Width = 1920;
	Height = 1080;

	mWindow = glfwCreateWindow(Width, Height, "My Open GL", NULL, NULL);

	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(0);
	InitGlad();

	glViewport(0, 0, Width, Height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplGlfw_InitForOpenGL(mWindow, false);
	ImGui_ImplOpenGL3_Init("#version 130");

	std::cout << "Hello" << std::endl;


	return 1;
}

//GLFWwindow* Window::GetMyGLFWwindow()
//{
//	std::cout << "Hello";
//	return mWindow;
//}

void Window::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImguiSetup();
}

void Window::EndFrame()
{
	/*bool Open = false;
	ImGui::ShowDemoWindow(&Open);*/

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Window::WindowFramebufferSizeCallback(int width, int height)
{
}

void Window::WindowMouseMoveCallback(double xpos, double ypos)
{
	//if (ImGui::GetIO().WantCaptureMouse) return;
}

void Window::WindowMouseButtonCallback(int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(mWindow, button, action, mods);
	if (ImGui::GetIO().WantCaptureMouse) return;
}

void Window::WindowMouseScrollCallback(double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(mWindow, xoffset, yoffset);
	if (ImGui::GetIO().WantCaptureMouse) return;
}

void Window::WindowKeyCallback(int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(mWindow, key, scancode, action, mods);
}

void Window::ImguiSetup()
{
	ImGui::Begin("Lines");

	bool test = false;
	if (ImGui::Button("Debug before!"))
	{
		Application::get().EnableDrawDebugBefore = true;
		Application::get().EnableDrawDebugAfter = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Debug after!"))
	{
		Application::get().EnableDrawDebugBefore = false;
		Application::get().EnableDrawDebugAfter = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Debug never!"))
	{
		Application::get().EnableDrawDebugBefore = false;
		Application::get().EnableDrawDebugAfter = false;
	}

	bool Activate = dynamic_cast<Enemy*>(Application::get().GetActor("EnemyActor"))->Active;
	ImGui::Checkbox("Enemy Activate", &Activate);
	dynamic_cast<Enemy*>(Application::get().GetActor("EnemyActor"))->Active = Activate;
	

	float WalkSpeed = Application::get().character->CharacterSpeed;
	ImGui::SliderFloat("WalkSpeed", &WalkSpeed, 0.5, 200);

	Application::get().character->CharacterSpeed = WalkSpeed;

	float dt = Application::get().deltaTime;
	int Frames = static_cast<int>(1.f / dt);
	std::string fps = "FPS: ";
	fps += std::to_string(Frames);

	ImGui::Text(fps.c_str());

	testQueue.push_back(Frames);
	// COLOR OF LINE
	testcolor[0] = Application::get().curve_mesh->Color.x;
	testcolor[1] = Application::get().curve_mesh->Color.y;
	testcolor[2] = Application::get().curve_mesh->Color.z;
	ImGui::ColorEdit3("color", testcolor);

	Application::get().curve_mesh->Color.x = testcolor[0];
	Application::get().curve_mesh->Color.y = testcolor[1];
	Application::get().curve_mesh->Color.z = testcolor[2];


	
	ImGui::PlotLines("FrameRate", testQueue.data(), 100);

	if (testQueue.size() >= 100)
	{
		testQueue.erase(testQueue.begin());
	}

	ImGui::End();

	ImGui::Begin("LandscapeGenerator");



	ImGui::SliderFloat("SizeX", &MaxX, 1, 100);
	ImGui::SliderFloat("SizeY", &MaxY, 1, 100);
	ImGui::SliderFloat("Delta", &Delta, 0.05f, 5);

	if (ImGui::Button("Generate"))
	{
		Application::get().RemakeLandscape(MaxX, MaxY, Delta);
	}
	//Application::get().RemakeLandscape(MaxX, MaxY, Delta);

	ImGui::End();
}

void Window::PrintHello()
{
	
	std::cout << mWindow << std::endl;
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

