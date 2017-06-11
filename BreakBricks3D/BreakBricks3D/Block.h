#pragma once
#include "GameObject.h"

class Block : public GameObject
{
public:
	Block();
	bool getActive();
	void setActive(bool active);
	bool getInvincible();
	void setHp(const int hp);
	void getDamaged(const int amount = 1);
	void render();

private:
	int hp;
	bool isActive, isInvincible;
};