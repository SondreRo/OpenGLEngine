#include <Enemy.h>

#include "Application.h"

Enemy::Enemy()
{
	CharacterSpeed = 5;
}

void Enemy::Update(glm::mat4 ParentMat)
{
	Character::Update(ParentMat);
	EnemyUpdate();
}

void Enemy::EnemyUpdate()
{
	if (!Active) return;
	glm::vec3 PlayerPos = Application::get().character->GlobalTransform.GetPosition();
	glm::vec3 DirectionVector = PlayerPos - GlobalTransform.GetPosition();

	DirectionVector.y = 0;
	DirectionVector = glm::normalize(DirectionVector);

	Velocity += DirectionVector * (Application::get().deltaTime * CharacterSpeed);
}
