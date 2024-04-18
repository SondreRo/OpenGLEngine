#pragma once
#include <vector>
#include <Vertex.h>
#include <Transform.h>
#include <Triangle.h>
#include <Texture.h>
#include <string>
#include <Material.h>

class ShaderProgram;

class Mesh {



public:

	Mesh() = default;

	Mesh(std::vector<Vertex> inVertex, std::vector<unsigned int> inIndices, std::vector<Texture> inTextures);

	std::string DisplayName;
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;

	virtual ~Mesh();

	Material material;

	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;

	virtual int Bind();
	virtual int RebindVertex();
	virtual void Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix);
	virtual void Tick(float deltaTime);
	Transform GlobalTransform;
	std::vector <Triangle> triangles;
	void GenerateTriangles();

	void ReimportMesh();

	std::string path;

	bool DrawLines = false;
	bool UseElements = false;
};
