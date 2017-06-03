#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "GL2/GL2_World.h"
#include "GL2/GL2_Light.h"

// Input.h (the actual callback class for glfwSetMouseButtonCallback)
class Graphics
{
public:
	static Graphics& getInstance() // Singleton is accessed via getInstance()
	{
		static Graphics instance; // lazy singleton, instantiated on first use
		return instance;
	}

	int init();
	int mainLoop();
	void release();

	GL2_World& getGL_World();
	GL2_Light& getLight();

private:
	const int FPS = 60;
	const int width_window = 1200;
	const int height_window = 900;

	GLFWwindow *window = nullptr;
	GL2_Light light;
	GL2_World gl_world;

	Graphics(void) // private constructor necessary to allow only 1 instance
	{
	}

	Graphics(Graphics const&); // prevent copies
	void operator=(Graphics const&); // prevent assignments
};