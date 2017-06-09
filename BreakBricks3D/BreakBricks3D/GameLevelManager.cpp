#include "GameLevelManager.h"
#include "GameObjectManager.h"

void GameLevelManager::init()
{
	ifstream levelDataFile("Data\\level.dat");

	if (!levelDataFile)
	{
		cout << "Cannot read level.dat File!!" << endl;
		return;
	}

	// 변수 초기화
	levelDataFile >> totalLevel >> mapHeight >> mapWidth;

	while (totalLevel--)
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

void GameLevelManager::initBlocks(const int level)
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			char blockType = blockInfo[level][i * mapWidth + j];
			int matType;

			if (blockType == '0')
			{
				continue;
			}

			if (blockType >= '1' && blockType <= '3')
			{
				matType = blockType - '0' + 2;
			}
			else
			{
				matType = 1;
			}

			GameObjectManager::getInstance().addBlock(j, mapHeight - i, matType);
		}
	}
}
