#include "GameObjectManager.h"

const float GameObjectManager::BALL_RADIUS = 0.2f;

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
	ball.setScale(glm::vec3(BALL_RADIUS * 2, BALL_RADIUS * 2, BALL_RADIUS * 2));
}

GameObject & GameObjectManager::getPlayer()
{
	return player;
}

void GameObjectManager::startBall()
{
	ball.setVelocity(glm::vec3(0.0f, 0.5f, 0.0f));
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

	collisionCheck();
	ball.advanceOneTimeStep(0.1f);
	ball.render();

	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->render();
	}
}

void GameObjectManager::collisionCheck()
{
	const glm::vec3& ballPos = ball.getPosition();
	const glm::vec3& ballScale = ball.getScale();
	const float ballWidth = ballScale.x * 0.5f;
	const float ballHeight = ballScale.y * 0.5f;

	const glm::vec3& playerPos = player.getPosition();
	const glm::vec3& playerScale = player.getScale();
	const float playerWidth = playerScale.x * 0.5f;
	const float playerHeight = playerScale.y * 0.5f;

	const glm::vec3& upperLeftPos = player.getPosition() + glm::vec3(-playerWidth, playerHeight, 0);
	const glm::vec3& upperRightPos = player.getPosition() + glm::vec3(playerWidth, playerHeight, 0);
	const glm::vec3& lowerLeftPos = player.getPosition() + glm::vec3(-playerWidth, -playerHeight, 0);
	const glm::vec3& lowerRightPos = player.getPosition() + glm::vec3(playerWidth, -playerHeight, 0);

	// 공이 판에 부딪혀서 튕겨나온다.
	if (ballPos.y - ballHeight < playerPos.y + playerHeight)
	{
		const glm::vec3& ballVelocity = ball.getVelocity();
		if (ballVelocity.y < 0.0f)
		{
			// 판의 위치에 따라 다르게 튕겨나감
			const float newX = (ballPos.x - playerPos.x) * 0.5f;
			ball.setVelocity(glm::vec3(newX, -ballVelocity.y, ballVelocity.z));
		}
	}

	std::cout << ballPos.x << ' ' << ballPos.y << ' ' << ballPos.z << std::endl;
	std::cout << ballScale.x << ' ' << ballScale.y << ' ' << ballScale.z << std::endl;
	std::cout << playerPos.x << ' ' << playerPos.y << ' ' << playerPos.z << std::endl;
	std::cout << playerScale.x << ' ' << playerScale.y << ' ' << playerScale.z << std::endl << std::endl;

	// 블럭에 맞고 튕겨 나온다.
	for (int i = 0; i < blocks.size(); i++)
	{
		const glm::vec3& blockPos = blocks[i]->getPosition();
		const glm::vec3& blockScale = blocks[i]->getScale();
		const float blockWidth = blockScale.x * 0.5f;
		const float blockHeight = blockScale.y * 0.5f;

		if (blocks[i]->getActive() == false)
		{
			continue;
		}

		// 블럭에 맞고 튕겨 나온다.
		if (ballPos.y + ballHeight > blockPos.y - blockHeight
			&& (ballPos.x - ballWidth >= blockPos.x - blockWidth && ballPos.x + ballWidth <= blockPos.x + blockWidth))
		{
			const glm::vec3& ballVelocity = ball.getVelocity();
			if (ballVelocity.y > 0.0f)
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));

				// 블럭 파괴
				blocks[i]->setActive(false);
			}
		}
	}
}
