#include <LineMesh.h>
#include <ShaderProgram.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

LineMesh::LineMesh()
{
	DrawLines = true;
}

int LineMesh::Bind()
{
	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
	glEnableVertexAttribArray(1);
	
	
	return 1;
}

void LineMesh::Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix)
{
	if (mVertices.size() == 0)
	{
		return;
	}

	UpdateLines();

	glEnable(GL_DEPTH_TEST);
	shader_program.UseProgram();

	Color = glm::vec3(1, 0, 0);

	glUniformMatrix4fv(glGetUniformLocation(shader_program.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	
	glUniform3fv(glGetUniformLocation(shader_program.shaderProgram, "Color"), 1, glm::value_ptr(Color));

	glBindVertexArray(VAO);
	glPointSize(10);
	glLineWidth(10);

	Application::get().camActor->RenderFromCamera(shader_program);

	if (DrawLines)
	{
		glDrawArrays(GL_LINES, 0, mVertices.size());
	}
	else
	{
		if (DrawDots)
		{
			if (UseElements)
			{
				glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);

			}
			else
			{
				glDrawArrays(GL_POINTS, 0, mVertices.size());
			}
		}
		else
		{
			if (UseElements)
			{
				glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);

			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
			}
		}
	}


}

void LineMesh::UpdateLines()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_DYNAMIC_DRAW);
}

void LineMesh::RemoveAllLines()
{
	mVertices.clear();
	
}

void LineMesh::AddLine(Vertex p1, Vertex p2)
{
	mVertices.push_back(p1);
	mVertices.push_back(p2);
}

void LineMesh::AddLine(glm::vec3 p1, glm::vec3 p2)
{
	mVertices.push_back(p1);
	mVertices.push_back(p2);
}
