#pragma once
#include <glm/glm.hpp>

struct Vertex {
	Vertex() = default;
	Vertex(glm::vec3 inPosition)
	{
		mPosition = inPosition;
	
	}
	Vertex(glm::vec3 inPosition, glm::vec3 inNormal)
	{
		mPosition = inPosition;
		mNormal = inNormal;
	}
	Vertex(float inX, float inY, float inZ)
	{
		mPosition[0] = inX;
		mPosition[1] = inY;
		mPosition[2] = inZ;

		
	}
	glm::vec3 mPosition = glm::vec3(0);
	glm::vec3 mNormal = glm::vec3(0);
	glm::vec2 mTexCoord = glm::vec2(0);
};