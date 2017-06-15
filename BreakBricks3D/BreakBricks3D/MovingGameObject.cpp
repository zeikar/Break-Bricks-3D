#include "MovingGameObject.h"

MovingGameObject::MovingGameObject()
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 & MovingGameObject::getVelocity()
{
	return velocity;
}

void MovingGameObject::setVelocity(const glm::vec3 & v)
{
	velocity = v;
}

void MovingGameObject::advanceOneTimeStep(const float dt)
{
	//position += velocity * dt;
	translate(velocity * dt);
}