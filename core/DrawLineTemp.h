#pragma once
#include <Mesh.h>
#include <Application.h>

#include "glad/glad.h"

class DrawLineTemp {

public:
	static void DrawLine(glm::vec3 p1, glm::vec3 p2) {
	
		Mesh* newLineMesh = new Mesh();
		newLineMesh->mVertices.push_back(p1);
		newLineMesh->mVertices.push_back(p2);
		newLineMesh->DrawLines = true;
		newLineMesh->Bind();
		newLineMesh->Draw(Application::get().shaderProgram, false, glm::mat4(1));

		
		glDeleteVertexArrays(1, &newLineMesh->VAO);
		glDeleteBuffers(1, &newLineMesh->VBO);
		glDeleteBuffers(1, &newLineMesh->EBO);
		delete newLineMesh;
	}

};
