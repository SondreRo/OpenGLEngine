#pragma once
#include <Mesh.h>

class MeshGenerator {


public:
	static float Function1(float X, float Y);
	static float Function2(float X, float Y);
	static float Function3(float X, float Y);
	static float Function4(float X, float Y);


	static glm::vec3 GetNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	static std::vector<Vertex> GeneratePlane(float MinX, float MaxX, float MinY, float MaxY, float StepSize, int type);
	static std::vector<Vertex> GenerateNormal(Mesh oldMesh, float arrowSize);
	static Mesh* GenerateBox(glm::vec3 HalfSize, glm::vec3 Location);
};