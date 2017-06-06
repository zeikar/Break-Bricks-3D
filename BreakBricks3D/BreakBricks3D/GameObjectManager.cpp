#include "GameObjectManager.h"
#include "Physics.h"
#include "SoundManager.h"

const float GameObjectManager::BALL_RADIUS = 0.2f;
const float GameObjectManager::LEFT_WALL_POS = -1.5f;
const float GameObjectManager::RIGHT_WALL_POS = 5.5f;
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

	ballSpeed = 0.1f;
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
	ball.advanceOneTimeStep(ballSpeed);
	ball.render();

	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->render();
	}
	
	// particles
	for (int i = 0; i < particleSystems.size(); i++)
	{
		particleSystems[i]->advanceOneTimeStep(0.01f);
		particleSystems[i]->renderParticles();
	}

	// 파티클 일정 y좌표 이하로 떨어지면 제거
	if (particleSystems.empty() == false &&
		particleSystems[0]->isAbleToDelete)
	{
		delete particleSystems[0];
		particleSystems.pop_front();
	}
}

void GameObjectManager::collisionCheck()
{
	const glm::vec3& ballPos = ball.getPosition();
	const glm::vec3& playerPos = player.getPosition();

	const glm::vec3& ballVelocity = ball.getVelocity();

	const int collisionBallAndPlayer = Physics::intersectionBetweenCircleAndRect(ball, player);
	// 공이 판에 부딪혀서 튕겨나온다.
	if (collisionBallAndPlayer == Physics::COLLISION_UP)
	{
		if (ballVelocity.y < 0.0f)
		{
			// 판의 위치에 따라 다르게 튕겨나감
			const float newX = (ballPos.x - playerPos.x) * 0.5f;
			ball.setVelocity(glm::vec3(newX, -ballVelocity.y, ballVelocity.z));
		}
	}
	else if (collisionBallAndPlayer == Physics::COLLISION_LEFT || collisionBallAndPlayer == Physics::COLLISION_RIGHT)
	{
		if (ballVelocity.x < 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}

	// 공이 옆 벽에 부딪혀서 튕겨나온다.
	if (Physics::intersectionBetweenCircleAndRect(ball, leftWall) == Physics::COLLISION_RIGHT)
	{
		if (ballVelocity.x < 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}
	else if (Physics::intersectionBetweenCircleAndRect(ball, rightWall) == Physics::COLLISION_LEFT)
	{
		if (ballVelocity.x > 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}

	// 공이 위 벽에 부딪혀서 튕겨나온다.
	if (Physics::intersectionBetweenCircleAndRect(ball, upWall) == Physics::COLLISION_DOWN)
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
	
	/*
	for(it2 = uc.begin(); it2 != uc.end();)
{
   ...   
   if(...)
   {
      it2 = uc.erase(it2); 
   }
   else
   {
      ++it2;
   }
   ...
}
*/
	// 블럭에 맞고 튕겨 나온다.
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->getActive() == false)
		{
			continue;
		}

		// 블럭에 맞고 튕겨 나온다.
		const int collision = Physics::intersectionBetweenCircleAndRect(ball, *blocks[i]);
		// 위나 아래 면에서 충돌
		if (collision == Physics::COLLISION_UP || collision == Physics::COLLISION_DOWN)
		{
			std::cout << i << "COLLISION_UP COLLISION_DOWN" << std::endl;

			if ((collision == Physics::COLLISION_UP && ballVelocity.y < 0) ||
				(collision == Physics::COLLISION_DOWN && ballVelocity.y > 0))
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
		}
		// 옆 면에서 충돌
		else if (collision == Physics::COLLISION_LEFT || collision == Physics::COLLISION_RIGHT)
		{
			std::cout << i << "COLLISION_LEFT COLLISION_RIGHT" << std::endl;

			if ((collision == Physics::COLLISION_LEFT && ballVelocity.x > 0) ||
				(collision == Physics::COLLISION_RIGHT && ballVelocity.x < 0))
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}			
		}
		// 구석에서 충돌
		//else if (collision == Physics::COLLISION_LOWER_LEFT || collision == Physics::COLLISION_LOWER_RIGHT ||
		//	collision == Physics::COLLISION_UPPER_LEFT || collision == Physics::COLLISION_UPPER_RIGHT)
		//{
		//	//// reflection vector
		//	//// r = d - 2 (d . n) n

		//	//glm::vec3 rectCorner, normalVector;

		//	// lower left
		//	if (collision == Physics::COLLISION_LOWER_LEFT)
		//	{
		//		std::cout << i << "COLLISION_LOWER_LEFT" << std::endl;
		//	}
		//	// lower right
		//	else if (collision == Physics::COLLISION_LOWER_RIGHT)
		//	{
		//		std::cout << i << "COLLISION_LOWER_RIGHT" << std::endl;
		//	}
		//	// upper left
		//	else if (collision == Physics::COLLISION_UPPER_LEFT)
		//	{
		//		std::cout << i << "COLLISION_UPPER_LEFT" << std::endl;
		//	}
		//	// upper right
		//	else
		//	{
		//		std::cout << i << "COLLISION_UPPER_RIGHT" << std::endl;
		//	}

		//	//// 블럭 파괴
		//	//blocks[i]->setActive(false);
		//}
	}
}

void GameObjectManager::collisionBlock(GameObject* block, const Vector3D<float>& collisionPos)
{
	// 블럭 파괴
	block->setActive(false);

	addParticleSystem(collisionPos);

	// 블럭 파괴 효과음 재생
	SoundManager::getInstance().playBrickSound();
}

void GameObjectManager::addParticleSystem(const Vector3D<float>& collisionPos)
{
	particleSystems.push_back(new ParticleSystem(collisionPos));
}