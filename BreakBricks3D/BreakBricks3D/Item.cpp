#include "Item.h"

Item::Item()
{
	velocity = glm::vec3(0.0f, -1.0f, 0.0f);
	itemTTL = -1;
}

Item::~Item()
{
	applyItem(false);
}

void Item::initItem(Ball* ball, GameObject* player, const int itemType)
{
	this->ball = ball;
	this->player = player;
	this->itemType = itemType;
}

void Item::applyItem(bool apply)
{
	// 아이템 적용
	if (apply)
	{
		itemTTL = itemTime;
	}
	// 미적용인데 무시
	else if(itemTTL < 0)
	{
		return;
	}

	switch (itemType)
	{
	case POWER_UP:
	{
		if (apply)
		{
			ball->setPower(2);
			ball->setMaterial(GL2_Material::MAT_GOLD);
		}
		else
		{
			ball->setPower(1);
			ball->setMaterial(GL2_Material::MAT_RED);
		}
		break;
	}
	case SIZE_DOWN:
	{
		if (apply)
		{
			ball->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
			ball->setMaterial(GL2_Material::MAT_BLUE);
		}
		else
		{
			ball->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
			ball->setMaterial(GL2_Material::MAT_RED);
		}
		break;
	}
	case SIZE_UP:
	{
		if (apply)
		{
			ball->setScale(glm::vec3(2.0f, 2.0f, 2.0f));
			ball->setMaterial(GL2_Material::MAT_GREEN);
		}
		else
		{
			ball->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
			ball->setMaterial(GL2_Material::MAT_RED);
		}
		break;
	}
	default:
		break;
	}
}

void Item::render()
{
	// 아이템 사용 종료
	if (itemTTL == -2)
	{
		return;
	}

	// 아이템 아직 안먹었다 -> 화면에 있음
	if (itemTTL == -1)
	{
		// 아이템이 밑으로 떨어진다.
		advanceOneTimeStep(0.02f);

		GameObject::render();
	}
	// 아이템 사용 종료 -> 원래대로 복구
	else if (itemTTL == 0)
	{
		applyItem(false);
		itemTTL = -2;
	}
	// 아이템 사용 중
	else
	{
		--itemTTL;
	}
}

bool Item::isItemAvailable()
{
	return itemTTL == -1;
}
