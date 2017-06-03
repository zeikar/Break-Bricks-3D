#include "GameManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"

void GameManager::init()
{
	// 플레이어(판) 초기화
	GameObjectManager::getInstance().initPlayer();

	// 벽 초기화
	GameObjectManager::getInstance().initWalls();

	// 공 초기화
	GameObjectManager::getInstance().initBall();

	// 블럭 초기화
	initMap();
}

void GameManager::inputProcess()
{
	if (InputManager::getInstance().getKeyPressed(GLFW_KEY_A))
	{
		GameObjectManager::getInstance().getPlayer().translate(glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	else if (InputManager::getInstance().getKeyPressed(GLFW_KEY_D))
	{
		GameObjectManager::getInstance().getPlayer().translate(glm::vec3(0.1f, 0.0f, 0.0f));
	}

	// 스페이스바
	if (InputManager::getInstance().getKeyPressed(GLFW_KEY_SPACE))
	{
		if (isGameStarted == false) 
		{
			GameObjectManager::getInstance().startBall();
			isGameStarted = true;
		}
		//GameObjectManager::getInstance().getBlock(1, 1, MAP_WIDTH)->setActive(false);
	}
}

void GameManager::renderGameObjects()
{
	GameObjectManager::getInstance().renderAll();
}

void GameManager::initMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			GameObjectManager::getInstance().addBlock(i, j);
		}
	}
}
