#include <Application.h>
#include <iostream>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include <Window.h>

#include <ShaderProgram.h>
#include <Camera.h>
#include <Vertex.h>
#include <Mesh.h>
#include <MeshActor.h>
#include <MeshGenerator.h>

#include "glm/gtc/type_ptr.hpp"
#include "TriangleCollision.h"
#include <Enemy.h>
#include <CurveMesh.h>




Application& Application::get()
{
	if (nullptr == instance)
	{
		instance = new Application;
	}

	return *instance;
}

int Application::init()
{
	mWindow = new Window();

	mWindow->CreatemyWindow();

	
	shaderProgram.ReadShaderFile(SOURCE_DIRECTORY("core/shaders/VertexShader.txt"), SOURCE_DIRECTORY("core/shaders/FragmentShader.txt"));
	shaderProgram.CompileShaders();
	shaderProgram.CreateProgram();

	LineshaderProgram.ReadShaderFile(SOURCE_DIRECTORY("core/shaders/lineShaders/LineVertexShader.txt"), SOURCE_DIRECTORY("core/shaders/lineShaders/LineFragmentShader.txt"));
	LineshaderProgram.CompileShaders();
	LineshaderProgram.CreateProgram();

	glfwSetWindowUserPointer(mWindow->mWindow, this);

	SetupCallbacks();


	SetupMeshes();
	BindMeshes();
	SetupActors();

	return 1;
}
Mesh boxMesh;
int Application::Run()
{

	glm::vec3 LightRotation{ 0,-0.5,0 };
	float x = 0;

	
	Transform SceneTransform;


	curve_mesh = new CurveMesh();

	curve_mesh->Bind();



	while (!glfwWindowShouldClose(mWindow->mWindow))
	{
		mWindow->StartFrame();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		//system("cls");
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		x += deltaTime;
		processInput(mWindow->mWindow);
		glClearColor(0.5f, 0.5f, 1, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		

		int Frames = static_cast<int>(1 / deltaTime);

		//std::cout << Frames << " fps \n";

		for (auto actor : actors)
		{
			actor->Update(SceneTransform.GetTransformMatrix());
		}

		LightRotation = glm::normalize(glm::vec3(abs(cos(x)) * -1, -1, 0));

		//shaderProgram.UseProgram();
		//defaultCamera.RenderFromCamera(shaderProgram);

		if (camActor)
			camActor->RenderFromCamera(shaderProgram);

		unsigned int test3 = glGetUniformLocation(shaderProgram.shaderProgram, "lightDirection");
		glUniform3fv(test3, 1, glm::value_ptr((glm::vec3(0,-1,0))));
		unsigned int test4 = glGetUniformLocation(shaderProgram.shaderProgram, "time");
		
		float timetest = glfwGetTime();
		glUniform1f(test4, timetest);


		Actor* temp1 = GetActor("EnemyActor");
		Actor* temp2 = GetActor("CharacterActor");

		if (temp1 && temp2)
		{
			glm::vec3 p1 = temp1->GlobalTransform.GetPosition();
			glm::vec3 p3 = temp2->GlobalTransform.GetPosition();


			float Distance = glm::length(p3 - p1);
			glm::vec3 p2 = ((p1 * .5f) + (p3 * .5f)) + glm::vec3(0, Distance, 0);

			
			curve_mesh->DrawLine(p1, p2, p3, 0.01);
			curve_mesh->Draw(LineshaderProgram, false, glm::mat4(2));
		}

		

		if (line_mesh)
		{
			line_mesh->Draw(LineshaderProgram, false, glm::mat4(1));
		}

		line_mesh->RemoveAllLines();
		for (auto actor : actors)
		{
			actor->Render();
		}

	
		
		//SceneTransform.AddPosition(glm::vec3(0, 0, 0.5 * deltaTime));
		//SceneTransform.AddYaw(20*deltaTime);

		//mActors["BoxMeshActor"]->LocalTransform.AddPosition((mActors["BoxMeshActor"]->LocalTransform.GetForward()) * 10.0f * deltaTime);
		//mActors["BoxMeshActor"]->LocalTransform.AddYaw(-90 * deltaTime);

		mWindow->EndFrame();
		glfwPollEvents();
		glfwSwapBuffers(mWindow->mWindow);
	}
	Cleanup();
	return 1;
}

Actor* Application::GetActor(std::string ActorName)
{
	if (Actor* newActor = mActors[ActorName])
	{
		return newActor;
	}

	std::cout << "Cant find " << ActorName << std::endl;
	return nullptr;
}

Mesh* Application::GetMesh(std::string meshName)
{
	if (Mesh* newMesh = mMeshes[meshName])
	{
		return newMesh;
	}
	
	std::cout << "Cant find " << meshName << std::endl;
	return nullptr;

}

void Application::SetupMeshes()
{
	Mesh* landscapeMesh = new Mesh();
	landscapeMesh->mVertices = MeshGenerator::GeneratePlane(0, 40, 0, 40, 2);
	landscapeMesh->GenerateTriangles();
	mMeshes["LandscapeMesh"] = landscapeMesh;

	Mesh* boxMesh = MeshGenerator::GenerateBox(glm::vec3(1), glm::vec3(0));
	mMeshes["BoxMesh"] = boxMesh;

	Mesh* characterMesh = MeshGenerator::GenerateBox(glm::vec3(0.5,1,0.5), glm::vec3(0));
	mMeshes["CharacterMesh"] = characterMesh;


	line_mesh = new LineMesh();
	line_mesh->mVertices.emplace_back(glm::vec3(0));
	line_mesh->mVertices.emplace_back(glm::vec3(10));
	
}

void Application::BindMeshes()
{
	for (auto element : mMeshes)
	{
		std::cout << "Binding mesh: '" << element.first << "' " << std::endl;
		element.second->Bind();
	}

	line_mesh->Bind();
}

void Application::SetupActors()
{
	MeshActor* LandScapeMeshActor = new MeshActor();
	LandScapeMeshActor->Name = "LandscapeMeshActor";
	LandScapeMeshActor->SetupMesh(GetMesh("LandscapeMesh"));
	mActors["LandScapeMeshActor"] = LandScapeMeshActor;
	actors.push_back(LandScapeMeshActor);

	MeshActor* BoxActor = new MeshActor();
	BoxActor->Name = "BoxActor";
	BoxActor->SetupMesh(GetMesh("BoxMesh"));
	BoxActor->LocalTransform.AddPosition(glm::vec3(10, 1, 10));
	mActors["BoxMeshActor"] = BoxActor;

	LandScapeMeshActor->Children.push_back(BoxActor);


	character = new Character();
	character->Name = "Character";
	character->SetupCharacterMesh(GetMesh("CharacterMesh"));
	mActors["CharacterActor"] = character;
	actors.push_back(character);

	character->LocalTransform.SetPosition(glm::vec3(10,2,10));

	camActor = new CameraActor();
	camActor->Name = "cameraActor";
	//camActor->LocalTransform.SetPosition(glm::vec3(0, 2, 5));

	character->Children.push_back(camActor);


	Enemy* enemy = new Enemy();
	enemy->Name = "Enemy";
	enemy->SetupCharacterMesh(GetMesh("CharacterMesh"));
	mActors["EnemyActor"] = enemy;
	actors.push_back(enemy);
	enemy->GlobalTransform.SetPosition(glm::vec3(30, 2, 30));

}

void Application::SetupCallbacks()
{
	glfwSetFramebufferSizeCallback(mWindow->mWindow, [](GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app)
		{
			app->FramebufferSizeCallback(window, width, height);
		}
		});

	glfwSetCursorPosCallback(mWindow->mWindow, [](GLFWwindow* window, double xpos, double ypos) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app) app->MouseMoveCallback(window, xpos, ypos);
		});

	glfwSetMouseButtonCallback(mWindow->mWindow, [](GLFWwindow* window, int button, int action, int mods) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app) app->MouseButtonCallback(window, button, action, mods);
		});

	glfwSetScrollCallback(mWindow->mWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app) app->MouseScrollCallback(window, xoffset, yoffset);
		});

	glfwSetKeyCallback(mWindow->mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app) app->KeyCallback(window, key, scancode, action, mods);
		});

	//glfwSetFramebufferSizeCallback(window->mWindow, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window->mWindow,mouse_callback);
	//glfwSetMouseButtonCallback(window->mWindow, MouseButtonCallback);
	//glfwSetKeyCallback(window->mWindow, KeyCallback);
	//glfwSetScrollCallback(window->mWindow, MouseScrollCallback);
}


void Application::processInput(GLFWwindow* window)
{
	if (character)
		character->processInput(window);

	defaultCamera.processInput(window);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	float boxSpeed = 3;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		boxMesh.GlobalTransform.SetPosition(boxMesh.GlobalTransform.GetPosition() + glm::vec3(boxSpeed * deltaTime,0,0));

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		boxMesh.GlobalTransform.SetPosition(boxMesh.GlobalTransform.GetPosition() + glm::vec3(-boxSpeed * deltaTime, 0, 0));

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		boxMesh.GlobalTransform.SetPosition(boxMesh.GlobalTransform.GetPosition() + glm::vec3(0,0,boxSpeed * deltaTime));

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		boxMesh.GlobalTransform.SetPosition(boxMesh.GlobalTransform.GetPosition() + glm::vec3(0,0, -boxSpeed * deltaTime));


	//std::cout << boxMesh.GlobalTransform.GetPosition().x << std::endl;
	 
}

void Application::Cleanup()
{
	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	character->MouseButtonCallback(window, button, action, mods);

	mWindow->WindowMouseButtonCallback(button, action, mods);
}

void Application::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	mWindow->WindowMouseScrollCallback(xoffset, yoffset);
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	mWindow->WindowKeyCallback(key, scancode, action, mods);
	//mWindow->PrintHello();
}

void Application::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	character->mouseCallback(window, xpos, ypos);
	mWindow->WindowMouseMoveCallback(xpos, ypos);
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "FrameSizeCallback" << std::endl;
	Application::mWindow->Height = height;
	Application::mWindow->Width = width;
	glViewport(0, 0, width, height);
}

void Application::RemakeLandscape(float maxX, float maxY, float delta)
{
	Mesh* lm = GetMesh("LandscapeMesh");
	lm->mVertices = MeshGenerator::GeneratePlane(0, maxX, 0, maxY, delta);
	lm->GenerateTriangles();

	lm->Bind();
	
}
