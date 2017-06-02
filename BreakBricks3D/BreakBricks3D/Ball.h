#pragma once
#include "GameObject.h"

class Ball : public GameObject
{
public:
	Ball();

	glm::vec3& getVelocity();
	void setVelocity(const glm::vec3& v);
	void advanceOneTimeStep(const float dt);

private:

	glm::vec3 velocity;
};