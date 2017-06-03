#include "GameObjectManager.h"

const float GameObjectManager::BALL_RADIUS = 0.2f;
const float GameObjectManager::LEFT_WALL_POS = -1.0f;
const float GameObjectManager::RIGHT_WALL_POS = 5.0f;
const float GameObjectManager::UP_WALL_POS = 5.0f;
const float GameObjectManager::WALL_WIDTH = 0.4f;

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

void GameObjectManager::initWalls()
{
	leftWall.readOBJ("box.obj");
	leftWall.setMaterial(GL2_Material::MAT_GRAY);
	leftWall.translate(glm::vec3(LEFT_WALL_POS, 0.0f, 0.0f));
	leftWall.setScale(glm::vec3(WALL_WIDTH, UP_WALL_POS * 2 - WALL_WIDTH, 1.0f));

	rightWall.readOBJ("box.obj");
	rightWall.setMaterial(GL2_Material::MAT_GRAY);
	rightWall.translate(glm::vec3(RIGHT_WALL_POS, 0.0f, 0.0f));
	rightWall.setScale(glm::vec3(WALL_WIDTH, UP_WALL_POS * 2 - WALL_WIDTH, 1.0f));

	upWall.readOBJ("box.obj");
	upWall.setMaterial(GL2_Material::MAT_GRAY);
	upWall.translate(glm::vec3((LEFT_WALL_POS + RIGHT_WALL_POS) * 0.5f, UP_WALL_POS, 0.0f));
	upWall.setScale(glm::vec3((RIGHT_WALL_POS - LEFT_WALL_POS) + WALL_WIDTH, WALL_WIDTH, 1.0f));
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
	leftWall.render();
	rightWall.render();
	upWall.render();

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
	const glm::vec3& playerPos = player.getPosition();

	const glm::vec3& ballVelocity = ball.getVelocity();

	const int collisionBallAndPlayer = collisionBetweenCircleAndRect(ball, player);
	// 공이 판에 부딪혀서 튕겨나온다.
	if (collisionBallAndPlayer == COLLISION_Y)
	{
		if (ballVelocity.y < 0.0f)
		{
			// 판의 위치에 따라 다르게 튕겨나감
			const float newX = (ballPos.x - playerPos.x) * 0.5f;
			ball.setVelocity(glm::vec3(newX, -ballVelocity.y, ballVelocity.z));
		}
	}
	else if (collisionBallAndPlayer == COLLISION_X)
	{
		if (ballVelocity.x < 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}

	// 공이 옆 벽에 부딪혀서 튕겨나온다.
	if (collisionBetweenCircleAndRect(ball, leftWall) == COLLISION_X)
	{
		if (ballVelocity.x < 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}
	else if (collisionBetweenCircleAndRect(ball, rightWall) == COLLISION_X)
	{
		if (ballVelocity.x > 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}

	// 공이 위 벽에 부딪혀서 튕겨나온다.
	if (collisionBetweenCircleAndRect(ball, upWall) == COLLISION_Y)
	{
		if (ballVelocity.y > 0.0f)
		{
			ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
		}
	}

	/*std::cout << ballPos.x << ' ' << ballPos.y << ' ' << ballPos.z << std::endl;
	std::cout << ballScale.x << ' ' << ballScale.y << ' ' << ballScale.z << std::endl;
	std::cout << playerPos.x << ' ' << playerPos.y << ' ' << playerPos.z << std::endl;
	std::cout << playerScale.x << ' ' << playerScale.y << ' ' << playerScale.z << std::endl << std::endl;*/

	// 블럭에 맞고 튕겨 나온다.
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->getActive() == false)
		{
			continue;
		}

		// 블럭에 맞고 튕겨 나온다.
		const int collision = collisionBetweenCircleAndRect(ball, *blocks[i]);
		// 옆 면에서 충돌
		if (collision == COLLISION_X)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));

			// 블럭 파괴
			blocks[i]->setActive(false);
		}
		// 위나 아래 면에서 충돌
		else if (collision == COLLISION_Y)
		{
			ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));

			// 블럭 파괴
			blocks[i]->setActive(false);
		}
		// 구석에서 충돌
		//else if (collision == COLLISION_CORNER)
		//{
		//	std::cout << "COLLISION_CORNER" << std::endl;

		//	ball.setVelocity(glm::vec3(ballVelocity.x, ballVelocity.y, ballVelocity.z));

		//	// 블럭 파괴
		//	blocks[i]->setActive(false);
		//}
	}
}

int GameObjectManager::collisionBetweenCircleAndRect(GameObject& circleObject, GameObject& rectObject)
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

	// y축 방향으로 충돌
	if (circleDistanceX <= (rectScale.x * 0.5f))
	{ 
		return COLLISION_Y;
	}
	// x축 방향으로 충돌
	if (circleDistanceY <= (rectScale.y * 0.5f))
	{
		return COLLISION_X; 
	}

	// 코너로 충돌 시
	const float cornerDistance_sq = (circleDistanceX - rectScale.x * 0.5f) * (circleDistanceX - rectScale.x * 0.5f) +
		(circleDistanceY - rectScale.y * 0.5f) * (circleDistanceY - rectScale.y * 0.5f);

	if (cornerDistance_sq <= (circleRadius * circleRadius))
	{
		return COLLISION_CORNER;
	}
	else
	{
		return 0;
	}
}