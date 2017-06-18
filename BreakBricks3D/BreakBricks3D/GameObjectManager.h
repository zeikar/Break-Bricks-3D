#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Ball.h"
#include "Block.h"
#include "Item.h"
#include <vector>
#include <deque>

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

	static const float PLAYER_WIDTH, PLAYER_HEIGHT;
	static const float BALL_RADIUS;
	static const float LEFT_WALL_POS, RIGHT_WALL_POS, UP_WALL_POS, WALL_WIDTH;

	void loadPlayer();
	void loadBall();
	void initPlayer();
	void initBall();
	void initWalls();
	GameObject& getPlayer();
	// 게임 시작 -> 공을 위로 보낸다.
	void startBall();
	//void addGameObject(const GameObject& gameObject);
	void addBlock(const int x, const int y, const int matType, const int hp);
	Block* getBlock(const int x, const int y, const int MAP_WIDTH);
	void renderAll();
	void deleteAllObjects(bool allClear = false);
	// 게임 클리어인지 (블록 전부 파괴) or 게임 오버 (볼을 놓쳤을 때)
	bool isGameClear();
	bool isGameOver();

private:
	GameObjectManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	GameObjectManager(GameObjectManager const&); // prevent copies
	void operator=(GameObjectManager const&); // prevent assignments

	float ballSpeed;

	// 밑의 판과 블록을 부술 공
	GameObject player;
	Ball ball;

	// 옆과 위의 벽들
	GameObject leftWall, rightWall, upWall;
	std::vector<Block*> blocks;

	// 파티클
	std::deque<ParticleSystem*> particleSystems;

	// 아이템
	std::deque<Item*> items;

	// 충돌 체크
	void collisionCheck();
	// 공이 블럭과 부딪힘
	void collisionBlock(Block* block, const Vector3D<float>& collisionPos);
	// 아이템 생성
	void addItem(const const Vector3D<float>& collisionPos, const int itemType, const int matType);
};