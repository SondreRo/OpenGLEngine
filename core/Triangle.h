#pragma once

struct Triangle {
	Triangle() = default;

	Triangle(glm::vec3 inP1, glm::vec3 inP2, glm::vec3 inP3) {
		P1 = inP1;
		P2 = inP2;
		P3 = inP3;
	}


	glm::vec3 P1;
	glm::vec3 P2;
	glm::vec3 P3;


};