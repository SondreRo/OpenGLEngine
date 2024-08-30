#pragma once
#include <Mesh.h>

class LineMesh : public Mesh {

public:
	LineMesh();
	virtual int Bind();

	virtual void Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix) override;
	void UpdateLines();

	void RemoveAllLines();

	void AddLine(Vertex p1, Vertex p2);
	void AddLine(glm::vec3 p1, glm::vec3 p2);

	glm::vec3 Color = glm::vec3(1);
};