#include <iostream>
#include <Mesh.h>
#include <glad/glad.h>
#include <ShaderProgram.h>

#include "Application.h"
#include "glm/gtc/type_ptr.inl"

Mesh::Mesh(std::vector<Vertex> inVertex, std::vector<unsigned> inIndices, std::vector<Texture> inTextures)
{
	mVertices = inVertex;
	mIndices = inIndices;
	mTextures = inTextures;
}

Mesh::~Mesh()
{
	//std::cout << "Hello" << std::endl;
}

int Mesh::Bind()
{

	// VAO
	glGenBuffers(1, &VBO);

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glGenBuffers(1, &EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mPosition));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoord));
	glEnableVertexAttribArray(2);

	return 1;
}

int Mesh::RebindVertex()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);
	return 1;
}

void Mesh::Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix)
{
	glEnable(GL_DEPTH_TEST);
	shader_program.UseProgram();

	glUniformMatrix4fv(glGetUniformLocation(shader_program.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniform1i(glGetUniformLocation(shader_program.shaderProgram, "useTexture"), (int)!mTextures.empty());
	material.BindMaterial(shader_program);

	for (auto texture : mTextures)
	{
		glBindTexture(GL_TEXTURE_2D, texture.id);
	}
	if (mTextures.empty())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	glBindVertexArray(VAO);
	glPointSize(10);
	glLineWidth(10);
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

void Mesh::Tick(float deltaTime)
{
	//GlobalTransform.AddYaw(90 * deltaTime);
}

void Mesh::GenerateTriangles()
{
	triangles.clear();
	for (int i = 0; i < mVertices.size(); i += 3)
	{
		if (i+2 > mVertices.size())
		{
			return;
		}

		triangles.emplace_back(mVertices[i].mPosition, mVertices[i + 1].mPosition, (mVertices[i + 2].mPosition));

	}
	std::cout << triangles.size() << " triangles generated" << std::endl;
}

void Mesh::ReimportMesh()
{
	Application::get().CreateAndRegisterMesh(path, DisplayName);
}
