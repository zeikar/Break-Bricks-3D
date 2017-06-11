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

	// 게임이 시작되었는지 체크, 게임 오버 체크
	bool isGameStarted = false, isGameOver = false;


	void inputProcess();
	void renderGameObjects();

	void sleep(const int amount);
};
