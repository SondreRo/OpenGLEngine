#include <CurveMesh.h>
#include <ShaderProgram.h>

#include "Application.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

void CurveMesh::Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix)
{
	if (mVertices.size() == 0)
	{
		return;
	}

	UpdateLines();

	glEnable(GL_DEPTH_TEST);
	shader_program.UseProgram();

	//Color = glm::vec3(1, 0, 0);

	glUniformMatrix4fv(glGetUniformLocation(shader_program.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glUniform3fv(glGetUniformLocation(shader_program.shaderProgram, "Color"), 1, glm::value_ptr(Color));

	glBindVertexArray(VAO);
	glPointSize(10);
	glLineWidth(10);

	Application::get().camActor->RenderFromCamera(shader_program);

	if (DrawLines)
	{
		glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
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

glm::vec3 CurveMesh::interpolate(glm::vec3 from, glm::vec3 to, float percent)
{
	glm::vec3 difference = to - from;
	return from + (difference * percent);
}

void CurveMesh::DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float step)
{
	mVertices.clear();



	for (float i = 0; i < 1; i += step)
	{
		// The Green Line
		glm::vec3 Va = interpolate(p1, p2, i);
		glm::vec3 Vb = interpolate(p2, p3, i);

		// The Black Dot
		glm::vec3 x = interpolate(Va, Vb, i);
		

		mVertices.emplace_back(x);
	}
}
