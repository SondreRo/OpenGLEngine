#include <CameraActor.h>
#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "Window.h"

void CameraActor::Update(glm::mat4 ParentMat)
{
	//Actor::Update(ParentMat);
    GlobalTransform.SetTransformMatrix(ParentMat * LocalTransform.GetTransformMatrix());
    GlobalTransform.AddPosition(-GlobalTransform.GetForward() * cameraDistance)
	;
    GlobalTransform.SetScale(glm::vec3(1));
  /*  std::cout <<
        GlobalTransform.GetPosition().x << " " <<
        GlobalTransform.GetPosition().y << " " <<
        GlobalTransform.GetPosition().z << " " << std::endl;*/
    cameraPos = GlobalTransform.GetPosition();

    Transform tempTrans;
    tempTrans.SetTransformMatrix(ParentMat);
    LookAt = tempTrans.GetPosition();
}

void CameraActor::RecalculateDirections()
{
    cameraRight = glm::normalize(glm::cross(cameraUp, cameraFront));
}

void CameraActor::RenderFromCamera(ShaderProgram shader_program)
{
    RecalculateDirections();

    glm::mat4 view = glm::lookAt(cameraPos, LookAt, cameraUp);
    //glm::mat4 view = ;
    

    int screenWidth = Application::get().mWindow->Width;
    int screenHeight = Application::get().mWindow->Height;

    float screenWidthf = static_cast<float>(screenWidth);
    float screenHeightf = static_cast<float>(screenHeight);

    float aspect = screenWidthf / screenHeightf;

    aspect = abs(aspect);
    //std::cout << aspect << std::endl;

    glm::mat4 projection = glm::perspective(glm::radians(90.f), aspect, 0.1f, 100.0f);


    unsigned int test1 = glGetUniformLocation(shader_program.shaderProgram, "view");
    unsigned int test2 = glGetUniformLocation(shader_program.shaderProgram, "projection");
    unsigned int test3 = glGetUniformLocation(shader_program.shaderProgram, "viewPos");
    glUniformMatrix4fv(test1, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(test2, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(test3, 1, glm::value_ptr(GlobalTransform.GetPosition()));
}

void CameraActor::AddRotation(float Rotation)
{
    //std::cout << LocalTransform.GetCurrentPitch() << std::endl;
    if (LocalTransform.GetCurrentPitch() < -50 )
    {
	    if (Rotation < 0)
	    {
            Rotation = 0;
	    }
    }
    if (LocalTransform.GetCurrentPitch() > 50)
    {
        if (Rotation > 0)
        {
            Rotation = 0;
        }
    }
    LocalTransform.AddPitch(Rotation);
}
