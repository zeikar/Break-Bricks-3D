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
	void release();
	void inputProcess();
	void renderGameObjects();

private:
	GameManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	GameManager(GameManager const&); // prevent copies
	void operator=(GameManager const&); // prevent assignments

	static const int MAP_WIDTH = 7, MAP_HEIGHT = 7;

	// 게임이 시작되었는지 체크
	bool isGameStarted = false;

	// 화면에 블럭들을 초기화
	void initMap();
};
