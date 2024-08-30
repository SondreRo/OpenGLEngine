#include <MeshComponent.h>
#include <Application.h>

void MeshComponent::Render(glm::mat4 transform)
{
	Component::Render(transform);
	if (!mesh) return;
	mesh->Draw(Application::get().shaderProgram, false, transform);
}
