#pragma once

#include <Transform.h>
#include <Component.h>
#include <string>
#include <vector>

class Actor {
public:
	
	Actor() = default;
	Actor(std::string name)
	{
		Name = name;
	}
	std::string Name;
	Transform LocalTransform;
	Transform GlobalTransform;

	std::vector<Actor*> Children;
	std::vector<Component*> components;

	virtual void Setup();
	virtual void Update(glm::mat4 ParentMat);
	virtual void Render();
	

};
