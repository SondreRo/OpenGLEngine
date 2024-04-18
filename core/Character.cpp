#include <Character.h>
#include <iostream>

#include "Application.h"
#include "MeshActor.h"
#include "TriangleCollision.h"
#include "GLFW/glfw3.h"

Character::Character()
{
	CharacterMesh = new MeshComponent;
}

void Character::SetupCharacterMesh(Mesh* NewMesh)
{
	CharacterMesh->mesh = NewMesh;
	components.push_back(CharacterMesh);
}

void Character::processInput(GLFWwindow* window)
{
	float deltaTime = Application::get().deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Velocity += LocalTransform.GetForward() * 1.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Velocity -= LocalTransform.GetForward() * 1.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Velocity += LocalTransform.GetRight() * 1.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Velocity -= LocalTransform.GetRight() * 1.f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Jump();

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        Application::get().EnableDrawDebugAfter = true;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        Application::get().EnableDrawDebugBefore = true;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        Application::get().EnableDrawDebugBefore = false;
        Application::get().EnableDrawDebugAfter = false;
    }
     
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
    	Application::get().line_mesh->AddLine(Vertex(GlobalTransform.GetPosition()), Vertex(glm::vec3(0)));
    }


}

void Character::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (!UseCamera)
    {
        return;
    }

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float rotationSpeed = 90.f;
    xoffset *= rotationSpeed * Application::get().deltaTime;
    yoffset *= rotationSpeed * Application::get().deltaTime;

    LocalTransform.AddYaw(-xoffset);


    CameraActor* ca = Application::get().camActor;
    ca->AddRotation(yoffset);



    //pitch += yoffset;

  
}

void Character::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    //std::cout << "MouseButtonCallback" << std::endl;
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            UseCamera = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (action == GLFW_RELEASE)
        {
            UseCamera = false;
            firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void Character::Jump()
{
    if (startJump) return;

    startJump = true;
    Velocity.y = 1;
}

void Character::Update(glm::mat4 ParentMat)
{
	Actor::Update(ParentMat);
	//Velocity = glm::normalize(Velocity);
	LocalTransform.AddPosition(Velocity * (Application::get().deltaTime * CharacterSpeed));
	Velocity *= (1.f - (5 * (Application::get().deltaTime)));

    Actor* LandScapeActor = Application::get().GetActor("LandScapeMeshActor");
    float height = TriangleCollision::StartTriangleCollision(LandScapeActor, GlobalTransform.GetPosition() - glm::vec3(0, 1, 0));
    onFloor = (height >= GlobalTransform.GetPosition().y - 0.5);
    //std::cout << onFloor << std::endl;


    if (onFloor)
    {
   
        startJump = false;
        // std::cout << "YEEE" << std::endl;

        if (Velocity.y < 0)
        {
	        Velocity.y = 0;
           
        }


        if (Velocity.y > 0)
        {
            return;

        }
        if (startJump == false)
        {
            LocalTransform.SetPosition(glm::vec3(LocalTransform.GetPosition().x, height + 0.5, LocalTransform.GetPosition().z));

        }
    }
    else
    {
       
        Velocity.y -= Gravity * Application::get().deltaTime;
    }
	
}
