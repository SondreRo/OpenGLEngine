#pragma once
#include <Mesh.h>

class TriangleCollision
{
public:
	static float StartTriangleCollision(Mesh* inMesh, glm::vec3 pos);

	static glm::vec3 Barycentric(Triangle& triangle, glm::vec3 pos, float &height);

	static void Switch(glm::vec3& inVec);
};