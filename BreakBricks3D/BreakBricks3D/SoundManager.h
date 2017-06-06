#pragma once
#include "inc\fmod.h"

class SoundManager
{
public:
	// Singleton is accessed via getInstance()
	static SoundManager& getInstance()
	{
		// lazy singleton, instantiated on first use
		static SoundManager instance;
		return instance;
	}

	void init();
	void release();

	// 배경음악 재생
	void playBackgroundMusic();
	// 블럭 파괴될 때의 효과음 재생
	void playBrickSound();

private:
	SoundManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	SoundManager(SoundManager const&); // prevent copies
	void operator=(SoundManager const&); // prevent assignments

	// fmod library for sound effects

	FMOD_SYSTEM *g_System;		// 음악 및 소리 재생
	FMOD_SOUND *g_Sound[6];		// 배경음악(효과음)의 개수
	FMOD_CHANNEL *g_Channel[2]; // 동시재생될 개수
};