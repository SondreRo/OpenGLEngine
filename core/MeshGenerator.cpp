#include <MeshGenerator.h>

#include <iostream>

float MeshGenerator::Function1(float X, float Y)
{
	return sin(X/2) + cos(Y/2);
}
float MeshGenerator::Function2(float X, float Y)
{
	return sin(X/4)*4;
}
float MeshGenerator::Function3(float X, float Y)
{
	return cos((X * (Y / 10) )/5) + sin(Y/10)/5;
}
float MeshGenerator::Function4(float X, float Y)
{
	return sin(10 * (X * X + Y * Y)) / 10;
	//return (pow(X, -1) + (pow(Y, 2) / 10))/20;
}

glm::vec3 MeshGenerator::GetNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 A = p2 - p1;
	glm::vec3 B = p3 - p1;


	//float nX = A.y * B.z - A.z * B.y;
	//float nY = A.z * B.x - A.x * B.z;
	//float nZ = A.x * B.y - A.y * B.x;

	return -(glm::normalize(glm::cross(A, B)));
	//return glm::vec3(nX, nY, nZ);
}

std::vector<Vertex> MeshGenerator::GeneratePlane(float MinX, float MaxX, float MinZ, float MaxZ, float StepSize, int type)
{
	std::vector<Vertex> vertices;

	float X = MinX;
	float Y = MinZ;

	for (float X = MinX; X < MaxX; X += StepSize)
	{
		for (float Y = MinZ; Y < MaxZ; Y += StepSize)
		{
			float Z;

			float x1, x2, x3, x4;
			float y1, y2, y3, y4;
			float z1, z2, z3, z4;

			x1 = X - (StepSize / 2); // Bot Left
			x2 = X + (StepSize / 2); // Bot Right
			x3 = X - (StepSize / 2); // Top Left
			x4 = X + (StepSize / 2); // Top Right

			y1 = Y - (StepSize / 2); // Bot Left
			y2 = Y - (StepSize / 2); // Bot Right
			y3 = Y + (StepSize / 2); // Top Left
			y4 = Y + (StepSize / 2); // Top Right

			switch (type)
			{
			case 1:
				z1 = Function1(x1, y1); // Bot Left
				z2 = Function1(x2, y2); // Bot Right
				z3 = Function1(x3, y3); // Top Left
				z4 = Function1(x4, y4); // Top Right
				break;
			case 2:
				z1 = Function2(x1, y1); // Bot Left
				z2 = Function2(x2, y2); // Bot Right
				z3 = Function2(x3, y3); // Top Left
				z4 = Function2(x4, y4); // Top Right
				break;
			case 3:
				z1 = Function3(x1, y1); // Bot Left
				z2 = Function3(x2, y2); // Bot Right
				z3 = Function3(x3, y3); // Top Left
				z4 = Function3(x4, y4); // Top Right
				break;
			case 4:
				z1 = Function4(x1, y1); // Bot Left
				z2 = Function4(x2, y2); // Bot Right
				z3 = Function4(x3, y3); // Top Left
				z4 = Function4(x4, y4); // Top Right
				break;
			}


			glm::vec3 p1 = glm::vec3(x1, z1, y1);
			glm::vec3 p2 = glm::vec3(x2, z2, y2);
			glm::vec3 p3 = glm::vec3(x3, z3, y3);
			glm::vec3 p4 = glm::vec3(x4, z4, y4);


			// Triangle 1
			glm::vec3 normal1 = GetNormal(p1, p2, p3);
			vertices.emplace_back(p1, normal1);
			vertices.emplace_back(p2, normal1);
			vertices.emplace_back(p3, normal1);
		

			glm::vec3 normal2 = GetNormal(p2,p4, p3);
			// Triangle 2
			vertices.emplace_back(p2, normal2);
			vertices.emplace_back(p4, normal2);
			vertices.emplace_back(p3, normal2);
			

			

		}
	}
	return vertices;
}

std::vector<Vertex> MeshGenerator::GenerateNormal(Mesh oldMesh, float arrowSize)
{
	std::vector<Vertex> vertices;
	for (auto vert : oldMesh.mVertices)
	{
		glm::vec3 pos1 = vert.mPosition;
		glm::vec3 pos2 = vert.mPosition + (vert.mNormal * arrowSize);

		vertices.push_back(pos1);
		vertices.push_back(pos2);
	}
	
	return vertices;
}

Mesh* MeshGenerator::GenerateBox(glm::vec3 HalfSize, glm::vec3 Location)
{
	glm::vec3 v1 = { -0.5f * HalfSize.x, -0.5f * HalfSize.y, -0.5f * HalfSize.z };
	glm::vec3 v2 = { 0.5f * HalfSize.x, -0.5f * HalfSize.y, -0.5f * HalfSize.z };
	glm::vec3 v3 = { 0.5f * HalfSize.x, 0.5f * HalfSize.y, -0.5f * HalfSize.z };
	glm::vec3 v4 = { -0.5f * HalfSize.x, 0.5f * HalfSize.y, -0.5f * HalfSize.z };
	glm::vec3 v5 = { -0.5f * HalfSize.x, -0.5f * HalfSize.y, 0.5f * HalfSize.z };
	glm::vec3 v6 = { 0.5f * HalfSize.x, -0.5f * HalfSize.y, 0.5f * HalfSize.z };
	glm::vec3 v7 = { 0.5f * HalfSize.x, 0.5f * HalfSize.y, 0.5f * HalfSize.z };
	glm::vec3 v8 = { -0.5f * HalfSize.x, 0.5f * HalfSize.y, 0.5f * HalfSize.z };

	std::vector<Vertex> Vertices;
	Vertices.emplace_back(v1, glm::normalize(v1));
	Vertices.emplace_back(v2, glm::normalize(v2));
	Vertices.emplace_back(v3, glm::normalize(v3));
	Vertices.emplace_back(v4, glm::normalize(v4));
	Vertices.emplace_back(v5, glm::normalize(v5));
	Vertices.emplace_back(v6, glm::normalize(v6));
	Vertices.emplace_back(v7, glm::normalize(v7));
	Vertices.emplace_back(v8, glm::normalize(v8));

	Mesh* boxMesh = new Mesh;

	boxMesh->mVertices = Vertices;
	boxMesh->mIndices = {
		0, 1, 2,	 0, 2, 3,
		1, 5, 6,	 1, 6, 2,
		5, 4, 7,	 5, 7, 6,
		4, 0, 3,	 4, 3, 7,
		3, 2, 6,	 3, 6, 7,
		4, 5, 1,	 4, 1, 0 };
	boxMesh->UseElements = true;
	return boxMesh;
}
