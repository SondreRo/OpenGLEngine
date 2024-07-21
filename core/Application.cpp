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

#include "AssetManager.h"


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
	
	shaderProgram.ReadShaderFile(SOURCE_DIRECTORY("core/shaders/VertexShader.vert"), SOURCE_DIRECTORY("core/shaders/FragmentShader.frag"));
	shaderProgram.CompileShaders();
	shaderProgram.CreateProgram();

	LineshaderProgram.ReadShaderFile(SOURCE_DIRECTORY("core/shaders/lineShaders/LineVertexShader.vert"), SOURCE_DIRECTORY("core/shaders/lineShaders/LineFragmentShader.frag"));
	LineshaderProgram.CompileShaders();
	LineshaderProgram.CreateProgram();

	glfwSetWindowUserPointer(mWindow->mWindow, this);

	SetupCallbacks();

	//ImGui_ImplGlfw_InstallCallbacks(mWindow->mWindow);


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


	//curve_mesh = new CurveMesh();

	//curve_mesh->Bind();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	while (!glfwWindowShouldClose(mWindow->mWindow))
	{
	



		
	
		mWindow->StartFrame();
		processInput(mWindow->mWindow);
		//system("cls");
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		x += deltaTime;
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


		//Actor* temp1 = GetActor("EnemyActor");
		//Actor* temp2 = GetActor("CharacterActor");

		//if (temp1 && temp2)
		//{
		//	glm::vec3 p1 = temp1->GlobalTransform.GetPosition();
		//	glm::vec3 p3 = temp2->GlobalTransform.GetPosition();


		//	float Distance = glm::length(p3 - p1);
		//	glm::vec3 p2 = ((p1 * .5f) + (p3 * .5f)) + glm::vec3(0, Distance, 0);

		//	
		//	/*curve_mesh->DrawLine(p1, p2, p3, 0.01);
		//	curve_mesh->Draw(LineshaderProgram, false, glm::mat4(2));*/
		//}

		

		if (line_mesh)
		{
			line_mesh->Draw(LineshaderProgram, false, glm::mat4(1));
		}

		line_mesh->RemoveAllLines();
		for (auto actor : actors)
		{
			actor->Render();
		}
		
		//std::cout << ImGui::GetIO().WantCaptureMouse << std::endl;
		//std::cout << ImGui::GetIO().WantCaptureKeyboard << std::endl;
		
		
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
	if (mMeshes.count(meshName) > 0)
	{
		return mMeshes[meshName];
	}
	
	std::cout << "Cant find " << meshName << std::endl;
	return nullptr;

}

void Application::SetupMeshes()
{

	Mesh* landscapeMesh = new Mesh();
	landscapeMesh->mVertices = MeshGenerator::GeneratePlane(0, 40, 0, 40, 2, 1);
	landscapeMesh->GenerateTriangles();
	landscapeMesh->DisplayName = "LandscapeMesh";
	mMeshes["LandscapeMesh"] = landscapeMesh;

	Mesh* boxMesh = MeshGenerator::GenerateBox(glm::vec3(1), glm::vec3(0));
	boxMesh->DisplayName = "BoxMesh";
	mMeshes["BoxMesh"] = boxMesh;

	Mesh* characterMesh = MeshGenerator::GenerateBox(glm::vec3(0.5,1,0.5), glm::vec3(0));
	characterMesh->DisplayName = "CharacterMesh";
	mMeshes["CharacterMesh"] = characterMesh;


	mesh_importer.ImportAssimp(SOURCE_DIRECTORY("Meshes/Suzanne.fbx"));

	mesh_importer.ImportAssimp(SOURCE_DIRECTORY("Meshes/House.fbx"));

	mesh_importer.ImportAssimp(SOURCE_DIRECTORY("Meshes/Box.fbx"));
	

	Mesh* triangleMesh = new Mesh();
	triangleMesh->mVertices = {
		Vertex(glm::vec3(-0.5,-0.5,0), glm::vec3(0,1,0)),
		Vertex(glm::vec3(0.5,-0.5,0), glm::vec3(0,1,0)),
		Vertex(glm::vec3(0,0.5,0), glm::vec3(0,1,0))};

	triangleMesh->DisplayName = "TriangleMesh";
	triangleMesh->mIndices = { 0,1,2 };
	triangleMesh->UseElements = true;
	mMeshes["TriangleMesh"] = triangleMesh;


	mesh_importer.ImportAssimp("C:/Users/soroe/Documents/Blender/PawnTextureThingy.fbx");
	





	line_mesh = new LineMesh();
	line_mesh->mVertices.emplace_back(glm::vec3(0));
	line_mesh->mVertices.emplace_back(glm::vec3(10));


	//AssetManager::CreateFileFromContentFolder();
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

	/*MeshActor* MonkeyActor = new MeshActor();
	MonkeyActor->Name = "MonkeyActor";
	MonkeyActor->SetupMesh(GetMesh("MonkeyMesh"));
	MonkeyActor->LocalTransform.AddPosition(glm::vec3(10, 1, 10));
	mActors["MonkeyActor"] = MonkeyActor;
	LandScapeMeshActor->Children.push_back(MonkeyActor);

	MeshActor* houseActor = new MeshActor();
	houseActor->Name = "HouseActor";
	houseActor->SetupMesh(GetMesh("HouseMesh"));
	houseActor->LocalTransform.AddPosition(glm::vec3(15, 1, 5));
	mActors["HouseActor"] = houseActor;
	LandScapeMeshActor->Children.push_back(houseActor);

	MeshActor* blenderBoxActor = new MeshActor();
	blenderBoxActor->Name = "blenderBoxActor";
	blenderBoxActor->SetupMesh(GetMesh("blenderBoxMesh"));
	blenderBoxActor->LocalTransform.AddPosition(glm::vec3(20, 1, 5));
	mActors["blenderBoxActor"] = blenderBoxActor;
	LandScapeMeshActor->Children.push_back(blenderBoxActor);

	MeshActor* TriangleActor = new MeshActor();
	TriangleActor->Name = "TriangleActor";
	TriangleActor->SetupMesh(GetMesh("TriangleMesh"));
	mActors["TriangleActor"] = TriangleActor;
	LandScapeMeshActor->Children.push_back(TriangleActor);*/

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


	/*Enemy* enemy = new Enemy();
	enemy->Name = "Enemy";
	enemy->SetupCharacterMesh(GetMesh("CharacterMesh"));
	mActors["EnemyActor"] = enemy;
	actors.push_back(enemy);
	enemy->GlobalTransform.SetPosition(glm::vec3(30, 2, 30));*/

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

	glfwSetCharCallback(mWindow->mWindow, [](GLFWwindow* window, unsigned int c) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (app) app->CharCallback(window, c);
		});


	//glfwSetFramebufferSizeCallback(window->mWindow, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window->mWindow,mouse_callback);
	//glfwSetMouseButtonCallback(window->mWindow, MouseButtonCallback);
	//glfwSetKeyCallback(window->mWindow, KeyCallback);
	//glfwSetScrollCallback(window->mWindow, MouseScrollCallback);
}


void Application::processInput(GLFWwindow* window)
{

	if (ImGui::GetIO().WantCaptureKeyboard) return;
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

	//if (action == 1)
	//{
	//	std::cout << key << std::endl;

	//}
	
	//mWindow->PrintHello();
}

void Application::CharCallback(GLFWwindow* window, unsigned c)
{
	mWindow->WindowCharCallback(window, c);
}

void Application::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (ImGui::GetIO().WantCaptureMouse) return;

	character->mouseCallback(window, xpos, ypos);
	mWindow->WindowMouseMoveCallback(xpos, ypos);
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "FrameSizeCallback" << std::endl;
	
	mWindow->Height = height;
	mWindow->Width = width;
	glViewport(0, 0, width, height);
}

void Application::RemakeLandscape(float maxX, float maxY, float delta, int type)
{
	Mesh* lm = GetMesh("LandscapeMesh");
	lm->mVertices = MeshGenerator::GeneratePlane(0, maxX, 0, maxY, delta, type);
	lm->GenerateTriangles();
	lm->RebindVertex();
	
}

Mesh* Application::CreateAndRegisterMesh(std::string Path, std::string DisplayName)
{
	std::vector<Mesh*> newMeshList = mesh_importer.ImportAssimp(Path);

	for (auto &newMesh : newMeshList)
	{

		if (!DisplayName.empty())
		{
			newMesh->DisplayName = DisplayName;

		}

		mMeshes[newMesh->DisplayName] = newMesh;


		newMesh->Bind();

	}


	
	return nullptr;
}

bool Application::AddToMeshList(Mesh* inMesh, std::string Name)
{
	if (mMeshes.count(Name) > 0)
	{
		if (inMesh->path == mMeshes[Name]->path)
		{
			inMesh->DisplayName = Name;
			mMeshes[Name] = inMesh;
			std::cout << "Overiding Mesh bcs same path" << std::endl;
			return true;
		}
		else
		{
			std::cout << Name << " already exists, trying to add 1" << std::endl;
			Name += "1";
			AddToMeshList(inMesh, Name);
			return false;
		}
		
	}
	else
	{
		inMesh->DisplayName = Name;
		mMeshes[Name] = inMesh;
		return true;
	}


}

bool Application::AddActorList(Actor* inActor, std::string Name)
{
	inActor->Name = Name;
	mActors[Name] = inActor;
	actors.push_back(inActor);
	return true;
}
