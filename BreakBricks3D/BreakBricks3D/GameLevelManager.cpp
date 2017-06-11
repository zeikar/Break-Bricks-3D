#include "GameLevelManager.h"
#include "GameObjectManager.h"

void GameLevelManager::init()
{
	ifstream levelDataFile("Data\\level.txt");

	if (!levelDataFile)
	{
		cout << "Cannot read level.dat File!!" << endl;
		return;
	}

	// 변수 초기화
	levelDataFile >> totalLevel >> mapHeight >> mapWidth;

	for (int i = 0; i < totalLevel; i++)
	{
		stringstream input;

		for (int i = 0; i < mapHeight; i++)
		{
			string temp;
			getline(levelDataFile, temp);

			// 그냥 엔터
			if (temp.empty())
			{
				i--;
				continue;
			}

			input << temp;
		}

		blockInfo.push_back(input.str());
	}
}

bool GameLevelManager::initNewLevel()
{
	if (currentLevel >= totalLevel)
	{
		return false;
	}

	GameObjectManager::getInstance().initPlayer();
	GameObjectManager::getInstance().initBall();

	initBlocks();

	++currentLevel;

	return true;
}

void GameLevelManager::initBlocks()
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			char blockType = blockInfo[currentLevel][i * mapWidth + j];
			int matType, hp;

			if (blockType == '0')
			{
				continue;
			}

			if (blockType >= '1' && blockType <= '3')
			{
				matType = blockType - '0' + 2;
				hp = blockType - '0';
			}
			else
			{
				matType = 1;
				hp = -1;
			}

			GameObjectManager::getInstance().addBlock(j, mapHeight - i, matType, hp);
		}
	}
}
