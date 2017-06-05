#include "Physics.h"

bool Physics::collisionBetweenCircleAndRect(GameObject & circleObject, GameObject & rectObject,
	const glm::vec3& originVelocity, glm::vec3& newVelocity)
{
	int intersect = intersectionBetweenCircleAndRect(circleObject, rectObject);

	if (intersect == COLLISION_LEFT || intersect == COLLISION_RIGHT)
	{
		newVelocity = originVelocity;
		newVelocity.x *= -1;		
	}
	else if (intersect == COLLISION_UP || intersect == COLLISION_DOWN)
	{
		newVelocity = originVelocity;
		newVelocity.y *= -1;
	}
	//else if (intersect == COLLISION_UPPER_LEFT || intersect == COLLISION_UPPER_RIGHT ||
	//	intersect == COLLISION_LOWER_LEFT || intersect == COLLISION_LOWER_RIGHT)
	//{
	//	const glm::vec3& rectPos = rectObject.getPosition();
	//	const glm::vec3& rectScale = rectObject.getScale();

	//	TV rectCorner = rectPos, normalVector;

	//	// upper left
	//	if (intersect == COLLISION_UPPER_LEFT)
	//	{
	//		rectCorner.x_ -= rectScale.x;
	//		rectCorner.y_ += rectScale.y;
	//	}
	//	// upper right
	//	else if (intersect == COLLISION_UPPER_RIGHT)
	//	{
	//		rectCorner.x_ += rectScale.x;
	//		rectCorner.y_ += rectScale.y;
	//	}
	//	// lower left
	//	else if (intersect == COLLISION_LOWER_LEFT)
	//	{
	//		rectCorner.x_ -= rectScale.x;
	//		rectCorner.y_ -= rectScale.y;
	//	}
	//	// lower right
	//	else
	//	{
	//		rectCorner.x_ += rectScale.x;
	//		rectCorner.y_ -= rectScale.y;
	//	}

	//	normalVector = (rectCorner - rectPos).getNormalized();


	//	// r = d - 2 (d . n) n
	//	TV temp = TV(originVelocity) - 2 * dotProduct(TV(originVelocity), normalVector) * normalVector;		


	//	newVelocity.x = temp.x_;
	//	newVelocity.y = temp.y_;
	//	newVelocity.z = temp.z_;
	//}
	else
	{
		newVelocity = originVelocity;
		return false;
	}

	return true;
}

int Physics::intersectionBetweenCircleAndRect(GameObject & circleObject, GameObject & rectObject)
{
	const glm::vec3& circlePos = circleObject.getPosition();
	const float circleRadius = circleObject.getScale().x * 0.5f;

	const glm::vec3& rectPos = rectObject.getPosition();
	const glm::vec3& rectScale = rectObject.getScale();

	const float circleDistanceX = abs(circlePos.x - rectPos.x);
	const float circleDistanceY = abs(circlePos.y - rectPos.y);


	if (circleDistanceX > (rectScale.x * 0.5f + circleRadius))
	{
		return 0;
	}
	if (circleDistanceY > (rectScale.y * 0.5f + circleRadius))
	{
		return 0;
	}

	// 위 아래에서 충돌
	if (circleDistanceX <= (rectScale.x * 0.5f))
	{
		if (circlePos.y > rectPos.y)
		{
			return COLLISION_UP;
		}
		else
		{
			return COLLISION_DOWN;
		}
	}
	// 왼쪽 오른쪽에서 충돌
	if (circleDistanceY <= (rectScale.y * 0.5f))
	{
		if (circlePos.x > rectPos.x)
		{
			return COLLISION_RIGHT;
		}
		else
		{
			return COLLISION_LEFT;
		}
	}

	// 코너로 충돌 시
	const float cornerDistance_sq = (circleDistanceX - rectScale.x * 0.5f) * (circleDistanceX - rectScale.x * 0.5f) +
		(circleDistanceY - rectScale.y * 0.5f) * (circleDistanceY - rectScale.y * 0.5f);

	if (cornerDistance_sq <= (circleRadius * circleRadius))
	{
		// upper left
		if (circlePos.x < rectPos.x && circlePos.y > rectPos.y)
		{
			return COLLISION_UPPER_LEFT;
		}
		// upper right
		else if (circlePos.x > rectPos.x && circlePos.y > rectPos.y)
		{
			return COLLISION_UPPER_RIGHT;
		}
		// lower left
		else if (circlePos.x < rectPos.x && circlePos.y < rectPos.y)
		{
			return COLLISION_LOWER_LEFT;
		}
		// lower right
		else
		{
			return COLLISION_LOWER_RIGHT;
		}
	}
	else
	{
		return 0;
	}
}
