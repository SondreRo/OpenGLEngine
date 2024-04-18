#pragma once
#include <string>

#include "glm/glm.hpp"

class Component {

public:
	std::string DisplayName;
	virtual void Setup();
	virtual void Update();
	virtual void Render(glm::mat4 transform);
};
