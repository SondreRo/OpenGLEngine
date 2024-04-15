#pragma once
#include <LineMesh.h>

class CurveMesh : public LineMesh
{
public:

	virtual void Draw(ShaderProgram shader_program, bool DrawDots, glm::mat4 matrix) override;


	glm::vec3 interpolate(glm::vec3 from, glm::vec3 to, float percent);

	void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float step);
};