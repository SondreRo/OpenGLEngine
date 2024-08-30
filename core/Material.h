#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ShaderProgram.h>


struct Material {

    void BindMaterial(ShaderProgram shader);
 

    glm::vec3 diffuse = glm::vec3(.6, .6, 0);
    glm::vec3 specular = glm::vec3(0.5);
    float shininess = 64.f;
};
