#pragma once
#include "GameObject.h"

class Physics
{
public:
	static const int COLLISION_UP = 1, COLLISION_DOWN = 2, COLLISION_LEFT = 3, COLLISION_RIGHT = 4,
		COLLISION_UPPER_LEFT = 5, COLLISION_UPPER_RIGHT = 6, COLLISION_LOWER_LEFT = 7, COLLISION_LOWER_RIGHT = 8;

	static bool collisionBetweenCircleAndRect(GameObject & circleObject, GameObject & rectObject,
		const glm::vec3& originVelocity, glm::vec3& newVelocity);
	static int intersectionBetweenCircleAndRect(GameObject& circleObject, GameObject& rectObject);

private:

	// Disallow creating an instance of this object
	Physics() {}
};