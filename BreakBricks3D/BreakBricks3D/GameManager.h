#pragma once

class GameManager
{
public:
	// Singleton is accessed via getInstance()
	static GameManager& getInstance()
	{
		// lazy singleton, instantiated on first use
		static GameManager instance;
		return instance;
	}

	void init();
	void update();
	void release();

private:
	GameManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	GameManager(GameManager const&); // prevent copies
	void operator=(GameManager const&); // prevent assignments

	// 현재 게임 레벨
	int currentLevel = 0;
	// 게임이 시작되었는지 체크
	bool isGameStarted = false;


	void inputProcess();
	void renderGameObjects();
};
