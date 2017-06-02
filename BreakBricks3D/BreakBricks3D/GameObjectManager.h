#pragma once
#include "GameObject.h"
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
	GameObject& getPlayer();
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

	GameObject player;
	std::vector<GameObject*> blocks;
};