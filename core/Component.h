#pragma once
#include "glm/glm.hpp"

class Component {

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render(glm::mat4 transform);
};
