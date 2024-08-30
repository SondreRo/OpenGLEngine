#pragma once
#include <Component.h>
#include <Mesh.h>

class MeshComponent : public Component {
public:
	Mesh* mesh;
	virtual void Render(glm::mat4 transform) override;
};