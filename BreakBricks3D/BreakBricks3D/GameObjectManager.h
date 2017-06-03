#pragma once
#include "GameObject.h"
#include "Ball.h"
#include <vector>

class GameObjectManager
{
public:
	// Singleton is accessed via getInstance()
	static GameObjectManager& getInstance()
	{
		// lazy singleton, instantiated on first use
		static GameObjectManager instance;
		return instance;
	}

	void initPlayer();
	void initBall();
	void initWalls();
	GameObject& getPlayer();
	// 게임 시작 -> 공을 위로 보낸다.
	void startBall();
	//void addGameObject(const GameObject& gameObject);
	void addBlock(const int x, const int y);
	GameObject* getBlock(const int x, const int y, const int MAP_WIDTH);
	void renderAll();

private:
	GameObjectManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	GameObjectManager(GameObjectManager const&); // prevent copies
	void operator=(GameObjectManager const&); // prevent assignments

	static const float BALL_RADIUS;
	static const float LEFT_WALL_POS, RIGHT_WALL_POS, UP_WALL_POS, WALL_WIDTH;
	static const int COLLISION_X = 1, COLLISION_Y = 2, COLLISION_CORNER = 3;

	// 밑의 판과 블록을 부술 공
	GameObject player;
	Ball ball;

	// 옆과 위의 벽들
	GameObject leftWall, rightWall, upWall;
	std::vector<GameObject*> blocks;

	// 충돌 체크
	void collisionCheck();
	int collisionBetweenCircleAndRect(GameObject& circleObject, GameObject& rectObject);
};