#include "GameObjectManager.h"

void GameObjectManager::initPlayer()
{
	player.readOBJ("MikuFigure.obj");
}

GameObject & GameObjectManager::getPlayer()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return player;
}

void GameObjectManager::addBlock(const int x, const int y)
{
	const float BLOCK_WIDTH = 1.0f, BLOCK_HEIGHT = 0.5f;

	GameObject* gameObject = new GameObject();
	gameObject->readOBJ("box.obj");

	glm::vec3 position(x * BLOCK_WIDTH, y * BLOCK_HEIGHT, 0.0f);
	gameObject->translate(position);

	gameObject->setScale(glm::vec3(BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_WIDTH));
	blocks.push_back(gameObject);
}

GameObject * GameObjectManager::getBlock(const int x, const int y, const int MAP_WIDTH)
{
	return blocks[x * MAP_WIDTH + y];
}

void GameObjectManager::renderAll()
{
	player.render();

	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->render();
	}
}
