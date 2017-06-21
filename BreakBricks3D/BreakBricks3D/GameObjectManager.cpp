#include "GameObjectManager.h"
#include "Physics.h"
#include "SoundManager.h"

const float GameObjectManager::PLAYER_WIDTH = 2.0f;
const float GameObjectManager::PLAYER_HEIGHT = 0.2f;
const float GameObjectManager::BALL_RADIUS = 0.2f;
const float GameObjectManager::WALL_WIDTH = 0.4f;
const float GameObjectManager::LEFT_WALL_POS = -0.5f - WALL_WIDTH * 0.5f;
const float GameObjectManager::RIGHT_WALL_POS = 6.5f + WALL_WIDTH * 0.5f;
const float GameObjectManager::UP_WALL_POS = 5.0f;

void GameObjectManager::loadPlayer()
{
	player.readOBJ("box.obj");
	player.setMaterial(GL2_Material::MAT_BLUE);
	player.setScale(glm::vec3(2.0f, 0.2f, 1.0f));
}

void GameObjectManager::loadBall()
{
	ball.readOBJ("sphere.obj");
	ball.setMaterial(GL2_Material::MAT_RED);
	ball.setScale(glm::vec3(BALL_RADIUS * 2, BALL_RADIUS * 2, BALL_RADIUS * 2));
}

void GameObjectManager::initPlayer()
{
	player.setPosition(glm::vec3((LEFT_WALL_POS + RIGHT_WALL_POS) * 0.5f, -UP_WALL_POS + 1.0f, 0.0f));
}

void GameObjectManager::initBall()
{
	ball.setPosition(glm::vec3((LEFT_WALL_POS + RIGHT_WALL_POS) * 0.5f, -UP_WALL_POS + 1.0f + BALL_RADIUS * 1.0f, 0.0f));
	ball.setVelocity(glm::vec3());
	ballSpeed = 0.25f;
}

void GameObjectManager::initWalls()
{
	leftWall.readOBJ("box_texture.obj", "Texture\\wall.bmp");
	leftWall.setMaterial(GL2_Material::MAT_SKYBLUE);
	leftWall.translate(glm::vec3(LEFT_WALL_POS, 0.0f, 0.0f));
	leftWall.setScale(glm::vec3(WALL_WIDTH, UP_WALL_POS * 2 - WALL_WIDTH, 1.0f));

	rightWall.readOBJ("box_texture.obj", "Texture\\wall.bmp");
	rightWall.setMaterial(GL2_Material::MAT_SKYBLUE);
	rightWall.translate(glm::vec3(RIGHT_WALL_POS, 0.0f, 0.0f));
	rightWall.setScale(glm::vec3(WALL_WIDTH, UP_WALL_POS * 2 - WALL_WIDTH, 1.0f));

	upWall.readOBJ("box_texture.obj", "Texture\\wall.bmp");
	upWall.setMaterial(GL2_Material::MAT_SKYBLUE);
	upWall.translate(glm::vec3((LEFT_WALL_POS + RIGHT_WALL_POS) * 0.5f, UP_WALL_POS, 0.0f));
	upWall.setScale(glm::vec3((RIGHT_WALL_POS - LEFT_WALL_POS) + WALL_WIDTH, WALL_WIDTH, 1.0f));

	backWall.readOBJ("box_texture.obj", "Texture\\wall.bmp");
	backWall.setMaterial(GL2_Material::MAT_SKYBLUE);
	backWall.translate(glm::vec3((LEFT_WALL_POS + RIGHT_WALL_POS) * 0.5f, 0.0f, -1.0f));
	backWall.setScale(glm::vec3((RIGHT_WALL_POS - LEFT_WALL_POS) + WALL_WIDTH, UP_WALL_POS * 2 - WALL_WIDTH, 1.0f));
}

GameObject & GameObjectManager::getPlayer()
{
	return player;
}

void GameObjectManager::startBall()
{
	ball.setVelocity(glm::vec3(0.0f, 0.5f, 0.0f));
}

void GameObjectManager::addBlock(const int x, const int y, const int matType, const int hp)
{
	const float BLOCK_WIDTH = 1.0f, BLOCK_HEIGHT = 0.5f;

	Block* gameObject = new Block();
	gameObject->readOBJ("box_texture.obj", "Texture\\frontend-large.bmp");
	gameObject->setMaterial(matType);
	gameObject->setHp(hp);

	glm::vec3 position(x * BLOCK_WIDTH, y * BLOCK_HEIGHT, 0.0f);
	gameObject->translate(position);

	gameObject->setScale(glm::vec3(BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_WIDTH));
	blocks.push_back(gameObject);
}

Block * GameObjectManager::getBlock(const int x, const int y, const int MAP_WIDTH)
{
	return blocks[x * MAP_WIDTH + y];
}

void GameObjectManager::renderAll()
{
	player.render();
	leftWall.render();
	rightWall.render();
	upWall.render();
	backWall.render();

	collisionCheck();
	ball.advanceOneTimeStep(ballSpeed);
	ball.render();

	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->render();
	}

	// items
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->render();
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

void GameObjectManager::deleteAllObjects(bool allClear)
{
	// delete blocks, particles, items

	if (allClear)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			delete blocks[i];
		}
		blocks.clear();
	}
	else
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->setActive(false);
		}
	}

	for (int i = 0; i < particleSystems.size(); i++)
	{
		delete particleSystems[i];
	}

	particleSystems.clear();

	for (int i = 0; i < items.size(); i++)
	{
		delete items[i];
	}

	items.clear();
}

bool GameObjectManager::isGameClear()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->getActive() == true && blocks[i]->getInvincible() == false)
		{
			return false;
		}
	}

	return true;
}

bool GameObjectManager::isGameOver()
{
	return ball.getPosition().y < player.getPosition().y - 2.0f;
}

void GameObjectManager::collisionCheck()
{
	const glm::vec3& ballPos = ball.getPosition();
	const glm::vec3& playerPos = player.getPosition();

	const glm::vec3& ballVelocity = ball.getVelocity();

	const int collisionBallAndPlayer = Physics::intersectionBetweenCircleAndRect(ball, player);
	// 공이 판에 부딪혀서 튕겨나온다.
	if (collisionBallAndPlayer == Physics::COLLISION_UP ||
		collisionBallAndPlayer == Physics::COLLISION_UPPER_LEFT || collisionBallAndPlayer == Physics::COLLISION_UPPER_RIGHT)
	{
		if (ballVelocity.y < 0.0f)
		{
			// 판의 위치에 따라 다르게 튕겨나감
			const float v = ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y;
			const float newX = sin((ballPos.x - playerPos.x) / player.getScale().x * 1.4f) * sqrt(v);
			const float newY = sqrt(v - newX * newX);
			ball.setVelocity(glm::vec3(newX, newY, ballVelocity.z));
		}
	}
	else if (collisionBallAndPlayer == Physics::COLLISION_LEFT)
	{
		if (ballVelocity.x > 0.0f)
		{
			ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
		}
	}
	else if (collisionBallAndPlayer == Physics::COLLISION_RIGHT)
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
	
	// 블럭에 맞고 튕겨 나온다.
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->getActive() == false)
		{
			continue;
		}

		// 블럭에 맞고 튕겨 나온다.
		const int collision = Physics::intersectionBetweenCircleAndRect(ball, *blocks[i]);

		// 구석에서 충돌
		if (collision == Physics::COLLISION_UPPER_LEFT)
		{
			std::cout << "COLLISION_UPPER_LEFT" << std::endl;

			if (abs(playerPos.x - ballPos.x) > abs(ballPos.y - playerPos.y) && ballVelocity.x > 0)
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
			else if (abs(playerPos.x - ballPos.x) <= abs(ballPos.y - playerPos.y) && ballVelocity.y < 0)
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
		}
		else if (collision == Physics::COLLISION_UPPER_RIGHT)
		{
			std::cout << "COLLISION_UPPER_RIGHT" << std::endl;

			if (abs(playerPos.x - ballPos.x) > abs(ballPos.y - playerPos.y) && ballVelocity.x < 0)
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
			else if (abs(playerPos.x - ballPos.x) <= abs(ballPos.y - playerPos.y) && ballVelocity.y < 0)
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
		}
		else if (collision == Physics::COLLISION_LOWER_LEFT)
		{
			std::cout << "COLLISION_LOWER_LEFT" << std::endl;

			if (abs(playerPos.x - ballPos.x) > abs(ballPos.y - playerPos.y) && ballVelocity.x > 0)
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
			else if (abs(playerPos.x - ballPos.x) <= abs(ballPos.y - playerPos.y) && ballVelocity.y > 0)
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
		}
		else if (collision == Physics::COLLISION_LOWER_RIGHT)
		{
			std::cout << "COLLISION_LOWER_RIGHT" << std::endl;

			if (abs(playerPos.x - ballPos.x) > abs(ballPos.y - playerPos.y) && ballVelocity.x < 0)
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
			else if (abs(playerPos.x - ballPos.x) <= abs(ballPos.y - playerPos.y) && ballVelocity.y > 0)
			{
				ball.setVelocity(glm::vec3(ballVelocity.x, -ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}
		}
		// 위나 아래 면에서 충돌
		else if (collision == Physics::COLLISION_UP || collision == Physics::COLLISION_DOWN)
		{
			std::cout << "COLLISION_UP COLLISION_DOWN" << std::endl;

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
			std::cout << "COLLISION_LEFT COLLISION_RIGHT" << std::endl;

			if ((collision == Physics::COLLISION_LEFT && ballVelocity.x > 0) ||
				(collision == Physics::COLLISION_RIGHT && ballVelocity.x < 0))
			{
				ball.setVelocity(glm::vec3(-ballVelocity.x, ballVelocity.y, ballVelocity.z));
				// 블럭 파괴
				collisionBlock(blocks[i], blocks[i]->getPosition());
			}			
		}

		// 아이템과 플레이어와의 충돌 감지
		for (int i = 0; i < items.size(); i++)
		{
			const int collisionItem = Physics::intersectionBetweenCircleAndRect(*items[i], player);

			// 아이템 사용
			if (items[i]->isItemAvailable() &&
				(collisionItem == Physics::COLLISION_UP ||
				collisionItem == Physics::COLLISION_UPPER_LEFT || collisionItem == Physics::COLLISION_UPPER_RIGHT ||
					collisionItem == Physics::COLLISION_LEFT || collisionItem == Physics::COLLISION_RIGHT))
			{
				items[i]->applyItem(true);
				
				SoundManager::getInstance().playSound(SoundManager::ITEM_APPLY);
			}
		}
	}
}

void GameObjectManager::collisionBlock(Block* block, const Vector3D<float>& collisionPos)
{
	// 블럭 공격
	//block->setActive(false);
	block->getDamaged(ball.getPower());
	
	// 블럭 파괴
	if (block->getActive() == false)
	{
		// 파티클 추가
		particleSystems.push_back(new ParticleSystem(collisionPos, block->getMaterial()));

		// 10% 확률로
		if (rand() % 10 == 0)
		{
			const int itemType = rand() % Item::ITEM_SIZE + 1;
			// 아이템 추가
			addItem(collisionPos, itemType);
		}

		SoundManager::getInstance().playSound(SoundManager::BLOCK_DESTROY);
	}
	else
	{
		SoundManager::getInstance().playSound(SoundManager::BLOCK_COLLISION);
	}
}

void GameObjectManager::addItem(const Vector3D<float>& collisionPos, const int itemType)
{
	Item* itemObject = new Item();
	itemObject->initItem(&ball, &player, itemType);
	itemObject->readOBJ("sphere.obj");

	// 아이템 색깔 설정
	int matType;

	switch (itemType)
	{
	case Item::POWER_UP:
	{
		matType = GL2_Material::MAT_GOLD;
		break;
	}
	case Item::BALL_SIZE_DOWN:
	{
		matType = GL2_Material::MAT_GREEN;
		break;
	}
	case Item::PLAYER_SIZE_UP:
	{
		matType = GL2_Material::MAT_BLUE;
		break;
	}
	default:
		break;
	}

	itemObject->setMaterial(matType);

	glm::vec3 position(collisionPos.x_, collisionPos.y_, collisionPos.z_);
	itemObject->translate(position);

	itemObject->setScale(glm::vec3(0.5f, 0.2f, 0.5f));
	items.push_back(itemObject);
}