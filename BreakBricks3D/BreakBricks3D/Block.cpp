#include "Block.h"

Block::Block()
{
	isActive = true;
}

bool Block::getActive()
{
	return isActive;
}

void Block::setActive(bool active)
{
	isActive = active;
}

bool Block::getInvincible()
{
	return isInvincible;
}

void Block::setHp(const int hp)
{
	// hp == -1이면 무적 블록
	if (hp == -1)
	{
		isInvincible = true;
	}

	this->hp = hp;
}

void Block::getDamaged(const int amount)
{
	if (isInvincible)
	{
		return;
	}

	hp -= amount;

	if (hp <= 0)
	{
		isActive = false;
	}
}

void Block::render()
{
	if (isActive == false)
	{
		return;
	}

	GameObject::render();
}
