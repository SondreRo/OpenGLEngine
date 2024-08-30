#include <Actor.h>

void Actor::Setup()
{
	for (auto actor : Children)
	{
		actor->Setup();
	}

	for (auto component: components)
	{
		component->Setup();
	}
}

void Actor::Update(glm::mat4 ParentMat)
{
	GlobalTransform.SetTransformMatrix(ParentMat * LocalTransform.GetTransformMatrix());
	for (auto childActor : Children)
	{
		std::string Name = childActor->Name;
		childActor->Update(GlobalTransform.GetTransformMatrix());
	}

	for (auto component : components)
	{
		component->Update();
	}
}

void Actor::Render()
{
	for (auto actor : Children)
	{
		actor->Render();
	}
	for (auto component : components)
	{
		component->Render(GlobalTransform.GetTransformMatrix());
	}
}
