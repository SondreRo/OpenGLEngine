#include <iostream>
#include <Mesh.h>
#include <glad/glad.h>
#include <ShaderProgram.h>

#include "glm/gtc/type_ptr.inl"

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
	

	return 1;
}

void Mesh::Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix)
{
	std::cout << VBO << " " << triangles.size() << std::endl;
	glEnable(GL_DEPTH_TEST);
	shader_program.UseProgram();

	glUniformMatrix4fv(glGetUniformLocation(shader_program.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


	
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
