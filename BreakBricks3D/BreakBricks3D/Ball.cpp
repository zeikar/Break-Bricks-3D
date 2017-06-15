#include "Ball.h"

Ball::Ball()
{
	power = 1;
}

void Ball::setPower(const int amount)
{
	power = amount;
}

int Ball::getPower()
{
	return power;
}
