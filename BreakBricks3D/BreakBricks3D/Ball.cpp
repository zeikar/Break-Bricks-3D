#include "Ball.h"

Ball::Ball()
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 & Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(const glm::vec3 & v)
{
	velocity = v;
}

void Ball::advanceOneTimeStep(const float dt)
{
	//position += velocity * dt;
	translate(velocity * dt);
}