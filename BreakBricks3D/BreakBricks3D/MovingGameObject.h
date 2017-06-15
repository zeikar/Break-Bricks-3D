#pragma once
#include "GameObject.h"

class MovingGameObject : public GameObject
{
public:
	MovingGameObject();

	glm::vec3& getVelocity();
	void setVelocity(const glm::vec3& v);
	void advanceOneTimeStep(const float dt);

protected:

	glm::vec3 velocity;
};