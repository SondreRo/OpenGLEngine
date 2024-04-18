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


	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_
	//io.ConfigFlags |= ImGuiConfigFlags_

	ImGui_ImplGlfw_InitForOpenGL(mWindow, false);
	ImGui_ImplOpenGL3_Init("#version 330");

	std::cout << "Hello" << std::endl;
	//ImGui_ImplGlfw_InstallCallbacks(mWindow);


	//ImGui::GetIO().WantCaptureMouse = true;
	//ImGui::GetIO().WantCaptureKeyboard = true;


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
	if (ImGui::GetIO().WantCaptureMouse) return;
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

void Window::WindowCharCallback(GLFWwindow* window, unsigned c)
{
	ImGui_ImplGlfw_CharCallback(window, c);
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

	ImGui::PlotLines("FrameRate", testQueue.data(), 100);

	// COLOR OF LINE
	testcolor[0] = Application::get().curve_mesh->Color.x;
	testcolor[1] = Application::get().curve_mesh->Color.y;
	testcolor[2] = Application::get().curve_mesh->Color.z;
	ImGui::ColorPicker3("color", testcolor);
	

	Application::get().curve_mesh->Color.x = testcolor[0];
	Application::get().curve_mesh->Color.y = testcolor[1];
	Application::get().curve_mesh->Color.z = testcolor[2];


	

	if (testQueue.size() >= 100)
	{
		testQueue.erase(testQueue.begin());
	}

	ImGui::End();


	MeshList();
	ActorList();
	Test();
	LandScapeGenerator();
	ImportMeshMenu();

	//ImGui::BeginMainMenuBar();
//ImGui::Button("Test1");
//ImGui::Button("Test2");
//ImGui::Button("Test3");
//ImGui::EndMainMenuBar();

	/*ImGui::Begin("LandscapeGenerator");





	ImGui::End();*/

}

void Window::Test()
{
	ImGui::Begin("Test");

	ImGui::End();
}

void Window::LandScapeGenerator()
{
	ImGui::Begin("LandscapeGenerator");


	ImGui::SliderFloat("SizeX", &MaxX, 1, 200);
	ImGui::SliderFloat("SizeY", &MaxY, 1, 200);
	ImGui::SliderFloat("Delta", &Delta, 0.05f, 5);
	ImGui::SliderInt("Type", &Type, 1, 4);

	if (ImGui::Button("Generate"))
	{
		Application::get().RemakeLandscape(MaxX, MaxY, Delta, Type);
	}
	//Application::get().RemakeLandscape(MaxX, MaxY, Delta);

	ImGui::End();
}

void Window::ImportMeshMenu()
{
	ImGui::Begin("MeshImporter");
	//ImGui::SetKeyboardFocusHere(0);

	ImGui::InputText("DisplayName", DisplayName, IM_ARRAYSIZE(DisplayName));
	std::string DisplayNameString = DisplayName;

	if (DisplayNameString.empty())
	{
		ImGui::Text("Default Mesh Name will be used");
	}

	ImGui::InputText("Mesh Path", ImportPath, IM_ARRAYSIZE(ImportPath));
	std::string ImportPathString = ImportPath;

	if (ImportPathString.empty())
	{
		ImGui::Text("Set the import path");
	}


	if (!ImportPathString.empty())
	{
		if (ImGui::Button("Import Mesh"))
		{
			Application::get().CreateAndRegisterMesh(ImportPathString, DisplayNameString);
			
			memset(ImportPath, 0, sizeof ImportPath);
			memset(DisplayName, 0, sizeof DisplayName);
		}

	}


	ImGui::End();
}

void Window::MeshList()
{

	ImGui::Begin("MeshList");

	ImGui::Text("MeshList");
	if (ImGui::BeginListBox("##MeshList"))
	{
		for (auto mesh : Application::get().mMeshes)
		{
			std::string meshName = mesh.first;
			if (ImGui::Button(meshName.c_str()))
			{
				SelectedMesh = mesh.second;
			}
			//ImGui::EndChild();
		}
		ImGui::EndListBox();
	}

	if (SelectedMesh)
	{
		std::string NameString = "Name: ";
		std::string VertString = "Vertices: ";
		std::string FaceString = "Faces: ";
		std::string IndicesString = "Indices: ";
		std::string VBOString = "VBO: ";
		std::string VAOString = "VAO: ";
		std::string EBOString = "EBO: ";

		NameString += SelectedMesh->DisplayName;
		VertString += std::to_string(SelectedMesh->mVertices.size());
		FaceString += std::to_string(SelectedMesh->triangles.size());
		IndicesString += std::to_string(SelectedMesh->mIndices.size());
		VBOString += std::to_string(SelectedMesh->VBO);
		VAOString += std::to_string(SelectedMesh->VAO);
		EBOString += std::to_string(SelectedMesh->EBO);

		ImGui::Text(NameString.c_str());

		ImGui::Text(VertString.c_str());
		ImGui::SameLine();
		ImGui::Text(FaceString.c_str());
		ImGui::SameLine();
		ImGui::Text(IndicesString.c_str());

		ImGui::Text(VBOString.c_str());
		ImGui::SameLine();
		ImGui::Text(VAOString.c_str());
		ImGui::SameLine();
		ImGui::Text(EBOString.c_str());

		// --- Material -- //

		// DIFFUSE
		float MaterialColor[3] = {
			SelectedMesh->material.diffuse.x,
			SelectedMesh->material.diffuse.y,
			SelectedMesh->material.diffuse.z
		};
	
		ImGui::ColorPicker3("Diffuse", MaterialColor);

		
		SelectedMesh->material.diffuse.x = MaterialColor[0];
		SelectedMesh->material.diffuse.y = MaterialColor[1];
		SelectedMesh->material.diffuse.z = MaterialColor[2];

		// SPECULAR

		float MaterialSpecular[3] = {
		SelectedMesh->material.specular.x,
		SelectedMesh->material.specular.y,
		SelectedMesh->material.specular.z
		};

		ImGui::ColorPicker3("Specular", MaterialSpecular);


		SelectedMesh->material.specular.x = MaterialSpecular[0];
		SelectedMesh->material.specular.y = MaterialSpecular[1];
		SelectedMesh->material.specular.z = MaterialSpecular[2];

		// Shininess

		float Shininess = SelectedMesh->material.shininess;

		ImGui::DragFloat("Shininess", &Shininess);

		SelectedMesh->material.shininess = Shininess;



		// RE Import
		if (!SelectedMesh->path.empty())
		{
			if (ImGui::Button("Reimport Mesh"))
			{
				SelectedMesh->ReimportMesh();
			}
		}

	}

	



	ImGui::End();
}

void Window::ActorList()
{
	ImGui::Begin("ActorList");

	ImGui::Text("Actor List");
	if (ImGui::BeginListBox("##Actor List"))
	{
		for (auto actor : Application::get().mActors)
		{
			std::string actorName = actor.first;
			if (ImGui::Button(actorName.c_str()))
			{
				SelectedActor = actor.second;

			}


			//ImGui::EndChild();
		}
		ImGui::EndListBox();
	}

	if (SelectedActor)
	{
		std::string NameString = "Name: ";
		NameString += SelectedActor->Name;
		ImGui::Text(NameString.c_str());



		// -- Position -- //
	
		std::string XPos = "Xpos: ";
		XPos += std::to_string(SelectedActor->GlobalTransform.GetPosition().x);
		std::string YPos = "Ypos: ";
		YPos += std::to_string(SelectedActor->GlobalTransform.GetPosition().y);
		std::string ZPos = "Zpos: ";
		ZPos += std::to_string(SelectedActor->GlobalTransform.GetPosition().z);

		float Position[3] = {
			SelectedActor->LocalTransform.GetPosition().x,
			SelectedActor->LocalTransform.GetPosition().y,
			SelectedActor->LocalTransform.GetPosition().z };

		ImGui::Text("Local Position");
		ImGui::DragFloat3("##Local Position", Position, 0.1f);
		SelectedActor->LocalTransform.SetPosition(glm::vec3(Position[0], Position[1], Position[2]));


	

		// -- SCALE -- //
		
		std::string Xscale = "Xscale: ";
		Xscale += std::to_string(SelectedActor->GlobalTransform.GetScale().x);
		std::string Yscale = "Yscale: ";
		Yscale += std::to_string(SelectedActor->GlobalTransform.GetScale().y);
		std::string Zscale = "Zscale: ";
		Zscale += std::to_string(SelectedActor->GlobalTransform.GetScale().z);

		float Scale[3] = {
		SelectedActor->LocalTransform.GetScale().x,
		SelectedActor->LocalTransform.GetScale().y,
		SelectedActor->LocalTransform.GetScale().z };

		ImGui::Text("Local Scale");
		ImGui::DragFloat3("##Local Scale", Scale, 0.1f);
		ImGui::SameLine(0);
		ImGui::Checkbox("Lock Scale", &LockScale);

		if (LockScale)
		{
			SelectedActor->LocalTransform.SetScale(glm::vec3(Scale[0], Scale[0], Scale[0]));

		}
		else
		{
			SelectedActor->LocalTransform.SetScale(glm::vec3(Scale[0], Scale[1], Scale[2]));

		}

	

		// -- Rotation -- //
	

		float Rotation[2] = {
		SelectedActor->LocalTransform.GetCurrentPitch(),
		SelectedActor->LocalTransform.GetCurrentYaw() };
		ImGui::Text("Local Rotation");

		ImGui::DragFloat2("##Local Rotation", Rotation);

		SelectedActor->LocalTransform.SetRotationFromPitch(Rotation[0]);
		SelectedActor->LocalTransform.SetRotationFromYaw(Rotation[1]);
		

		// -- END -- //



		ImGui::Text(XPos.c_str());
		
		ImGui::Text(YPos.c_str());
		
		ImGui::Text(ZPos.c_str());

		ImGui::Text(Xscale.c_str());

		ImGui::Text(Yscale.c_str());

		ImGui::Text(Zscale.c_str());

		if (ImGui::BeginListBox("Components: "))
		{
			for (auto component : SelectedActor->components)
			{
				ImGui::Button("Mesh Component");

				if (MeshComponent* mc = dynamic_cast<MeshComponent*>(component))
				{
					ImGui::BeginListBox("Mesh");
					std::string test = mc->mesh->DisplayName;
					if (ImGui::Button(test.c_str()))
					{
						SelectedMesh = mc->mesh;
					}

					if (SelectedMesh)
					{
						std::string ButtonName = "Swap Mesh with: " + mc->mesh->DisplayName;
						if (ImGui::Button(ButtonName.c_str()))
						{
							mc->mesh = SelectedMesh;

						}
					}
				
					
					ImGui::EndListBox();
				}


			}
			ImGui::EndListBox();
		}

		

	}

	


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

