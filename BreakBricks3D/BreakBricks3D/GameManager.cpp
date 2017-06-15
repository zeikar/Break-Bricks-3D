#include "GameManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "GameLevelManager.h"

void GameManager::init()
{
	// 게임 레벨 초기화
	GameLevelManager::getInstance().init();

	// 플레이어(판) 초기화
	GameObjectManager::getInstance().loadPlayer();

	// 공 초기화
	GameObjectManager::getInstance().loadBall();

	// 벽 초기화
	GameObjectManager::getInstance().initWalls();

	// 레벨 초기화
	GameLevelManager::getInstance().initNewLevel();

	// sound manager 초기화
	SoundManager::getInstance().init();

	// 배경음악 재생
	SoundManager::getInstance().playBackgroundMusic();
}

void GameManager::update()
{
	inputProcess();


	if (isGameOver)
	{
		return;
	}

	// 게임 클리어 확인
	if (GameObjectManager::getInstance().isGameClear())
	{
		SoundManager::getInstance().stopBackgroungMusic();
		SoundManager::getInstance().playSound(SoundManager::GAME_CLEAR);

		// 남아있는 블럭, 파티클들 제거
		GameObjectManager::getInstance().deleteAllObjects();

		sleep(4000);

		// 다음 레벨 초기화 , 다음 레벨 없으면 게임 종료
		if (GameLevelManager::getInstance().initNewLevel() == false)
		{
			isGameOver = true;
			return;
		}

		isGameStarted = false;

		// 배경음악 재생
		SoundManager::getInstance().playBackgroundMusic();
	}

	// 게임 오버인지 확인
	if (GameObjectManager::getInstance().isGameOver())
	{
		SoundManager::getInstance().stopBackgroungMusic();
		SoundManager::getInstance().playSound(SoundManager::GAME_OVER);

		sleep(3000);

		isGameOver = true;

		return;
	}

	renderGameObjects();
}

void GameManager::release()
{
	// 남아있는 블럭, 파티클들 제거
	GameObjectManager::getInstance().deleteAllObjects();
	// sound manager 해제
	SoundManager::getInstance().release();
}

void GameManager::inputProcess()
{
	if (InputManager::getInstance().getKeyPressed(GLFW_KEY_A) ||
		InputManager::getInstance().getKeyPressed(GLFW_KEY_LEFT))
	{
		GameObjectManager::getInstance().getPlayer().translate(glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	else if (InputManager::getInstance().getKeyPressed(GLFW_KEY_D) ||
		InputManager::getInstance().getKeyPressed(GLFW_KEY_RIGHT))
	{
		GameObjectManager::getInstance().getPlayer().translate(glm::vec3(0.1f, 0.0f, 0.0f));
	}


	// 스페이스바
	if (InputManager::getInstance().getKeyPressed(GLFW_KEY_SPACE))
	{
		if (isGameStarted == false) 
		{
			GameObjectManager::getInstance().startBall();
			isGameStarted = true;
		}
		//GameObjectManager::getInstance().getBlock(1, 1, MAP_WIDTH)->setActive(false);
	}

	// r키 누르면 처음부터 재시작
	if (InputManager::getInstance().getKeyPressed(GLFW_KEY_R))
	{
		if (isGameOver == true)
		{
			// 남아있는 블럭, 파티클들 제거
			GameObjectManager::getInstance().deleteAllObjects();
			GameLevelManager::getInstance().initNewLevel(true);

			isGameStarted = false;
			isGameOver = false;

			// 배경음악 재생
			SoundManager::getInstance().playBackgroundMusic();

		}
		//GameObjectManager::getInstance().getBlock(1, 1, MAP_WIDTH)->setActive(false);
	}
}

void GameManager::renderGameObjects()
{
	GameObjectManager::getInstance().renderAll();
}

void GameManager::sleep(const int amount)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(amount));
}
