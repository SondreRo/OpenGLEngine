#pragma once
#include <vector>
#include <Vertex.h>
#include <Transform.h>
#include <Triangle.h>
class ShaderProgram;

class Mesh {



public:
	unsigned int VBO, VAO, EBO;

	~Mesh();

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	virtual int Bind();
	virtual void Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix);
	virtual void Tick(float deltaTime);
	Transform GlobalTransform;
	std::vector <Triangle> triangles;
	void GenerateTriangles();

	bool DrawLines = false;
	bool UseElements = false;
};
