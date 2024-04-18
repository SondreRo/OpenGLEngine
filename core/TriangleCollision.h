#pragma once
#include <Mesh.h>

#include "Actor.h"

class TriangleCollision
{
public:
	static float StartTriangleCollision(Actor* inLandscape, glm::vec3 pos);

	static glm::vec3 Barycentric(Triangle& triangle, glm::vec3 pos, float &height);

	static void Switch(glm::vec3& inVec);
};
