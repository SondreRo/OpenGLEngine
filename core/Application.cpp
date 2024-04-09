#include <Application.h>
#include <iostream>

#include <Window.h>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include <ShaderProgram.h>
#include <Camera.h>
#include <Vertex.h>
#include <Mesh.h>
#include <MeshActor.h>
#include <MeshGenerator.h>


#include "assimp/code/Common/PolyTools.h"
#include "glm/gtc/type_ptr.hpp"
#include "TriangleCollision.h"


void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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
	window = new Window();

	window->CreateWindow();

	
	shaderProgram.ReadShaderFile(SOURCE_DIRECTORY("core/shaders/VertexShader.txt"), SOURCE_DIRECTORY("core/shaders/FragmentShader.txt"));
	shaderProgram.CompileShaders();
	shaderProgram.CreateProgram();


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

	while (!glfwWindowShouldClose(window->GetGLFWwindow()))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		//system("cls");
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		x += deltaTime;
		processInput(window->GetGLFWwindow());
		glClearColor(0.5f, 0.5f, 1, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		std::cout << 1 / deltaTime << " fps \n";

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
		//glUniform3fv(test3, 1, glm::value_ptr((LightRotation)));
		glUniform3fv(test3, 1, glm::value_ptr((glm::vec3(0,-1,0))));

		unsigned int test4 = glGetUniformLocation(shaderProgram.shaderProgram, "time");
		//glUniform3fv(test3, 1, glm::value_ptr((LightRotation)));
		float timetest = glfwGetTime();
		glUniform1f(test4, timetest);
		//TriangleCollision::StartTriangleCollision(newMesh, boxMesh);

		for (auto actor : actors)
		{
			actor->Render();
		}

		//SceneTransform.AddPosition(glm::vec3(0, 0, 0.5 * deltaTime));
		//SceneTransform.AddYaw(20*deltaTime);

		//mActors["BoxMeshActor"]->LocalTransform.AddPosition((mActors["BoxMeshActor"]->LocalTransform.GetForward()) * 10.0f * deltaTime);
		//mActors["BoxMeshActor"]->LocalTransform.AddYaw(-90 * deltaTime);

		glfwPollEvents();
		glfwSwapBuffers(window->GetGLFWwindow());
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
	landscapeMesh->mVertices = MeshGenerator::GeneratePlane(0, 20, 0, 20, 2);
	landscapeMesh->GenerateTriangles();
	mMeshes["LandscapeMesh"] = landscapeMesh;

	Mesh* boxMesh = MeshGenerator::GenerateBox(glm::vec3(1), glm::vec3(0));
	mMeshes["BoxMesh"] = boxMesh;

}

void Application::BindMeshes()
{
	for (auto element : mMeshes)
	{
		std::cout << "Binding mesh: '" << element.first << "' " << std::endl;
		element.second->Bind();
	}
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
	character->SetupCharacterMesh(GetMesh("BoxMesh"));
	mActors["CharacterActor"] = character;
	actors.push_back(character);

	character->LocalTransform.SetPosition(glm::vec3(10,2,10));

	camActor = new CameraActor();
	camActor->Name = "cameraActor";
	//camActor->LocalTransform.SetPosition(glm::vec3(0, 2, 5));

	character->Children.push_back(camActor);

}

void Application::SetupCallbacks()
{
	glfwSetFramebufferSizeCallback(window->GetGLFWwindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window->GetGLFWwindow(),mouse_callback);
	glfwSetMouseButtonCallback(window->GetGLFWwindow(), MouseButtonCallback);
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

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application::get().defaultCamera.MouseButtonCallback(window, button, action, mods);
	if (Character* character = Application::get().character)
	{
		character->MouseButtonCallback(window, button, action, mods);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application::get().defaultCamera.mouseCallback(window, xpos, ypos);
	if (Character* character = Application::get().character)
	{
		character->mouseCallback(window, xpos, ypos);
	}
	
}

void Application::Cleanup()
{
	glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "Changed Window Size" << std::endl;
	glViewport(0, 0, width, height);
	Application::get().window->Width = width;
	Application::get().window->Height = height;
}

