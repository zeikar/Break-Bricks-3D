#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include "Graphics.h"

// Input.h (the actual callback class for glfwSetMouseButtonCallback)
class InputManager
{
public:
	// Singleton is accessed via getInstance()
	static InputManager& getInstance()
	{
		// lazy singleton, instantiated on first use
		static InputManager instance; 
		return instance;
	}

	// this method is specified as glfw callback
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//this is the actual implementation of the callback method
	void mouseButtonCallbackImpl(GLFWwindow* window, int button, int action);
	void cursorPositionCallbackImpl(GLFWwindow* window, double xpos, double ypos);
	void scrollCallbackImpl(GLFWwindow* window, double xoffset, double yoffset);
	void keyCallbackImpl(GLFWwindow* window, int key, int action);

	// 키가 눌려있는지 확인
	bool getKeyPressed(int key);

private:
	// const

	// check if keyboard is pressed
	int keyPress[255];

	InputManager(void) // private constructor necessary to allow only 1 instance
	{
	}

	InputManager(InputManager const&); // prevent copies
	void operator=(InputManager const&); // prevent assignments
};