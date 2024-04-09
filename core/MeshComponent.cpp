#include <MeshComponent.h>
#include <Application.h>

void MeshComponent::Render(glm::mat4 transform)
{
	Component::Render(transform);
	mesh->Draw(Application::get().shaderProgram, false, transform);
}
