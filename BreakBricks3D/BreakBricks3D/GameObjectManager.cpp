#include "GameObjectManager.h"

void GameObjectManager::initPlayer()
{
	player.readOBJ("box.obj");
	player.setMaterial(GL2_Material::MAT_BLUE);
	player.translate(glm::vec3(0.0f, -5.0f, 0.0f));
	player.setScale(glm::vec3(2.0f, 0.2f, 1.0f));
}

void GameObjectManager::initBall()
{
	ball.readOBJ("sphere.obj");
	ball.setMaterial(GL2_Material::MAT_RED);
	ball.translate(glm::vec3(0.0f, -4.0f, 0.0f));
	ball.setScale(glm::vec3(0.4f, 0.4f, 0.4f));
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
	ball.render();

	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->render();
	}
}
