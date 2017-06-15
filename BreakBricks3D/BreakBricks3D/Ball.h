#pragma once
#include "MovingGameObject.h"

class Ball : public MovingGameObject
{
public:
	Ball();
	void setPower(const int amount);
	int getPower();

private:
	int power;
};