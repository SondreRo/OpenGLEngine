#pragma once
#include <Character.h>

class Enemy : public Character {
	
public:
	Enemy();
	virtual void Update(glm::mat4 ParentMat) override;
	void EnemyUpdate();
	
};