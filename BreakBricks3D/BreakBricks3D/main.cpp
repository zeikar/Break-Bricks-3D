/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "Graphics.h"
#include "GameManager.h"

int main(int argc, char *argv[])
{
	Graphics::getInstance().init();
	GameManager::getInstance().init();

	/* Loop until the user closes the window */
	while (Graphics::getInstance().mainLoop())
	{		
		GameManager::getInstance().update();
	}

	GameManager::getInstance().release();
	Graphics::getInstance().release();

	return 0;
}