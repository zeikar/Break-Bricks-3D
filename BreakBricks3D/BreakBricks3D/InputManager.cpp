#include "InputManager.h"

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//here we access the instance via the singleton pattern and forward the callback to the instance method
	getInstance().mouseButtonCallbackImpl(window, button, action);
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	getInstance().cursorPositionCallbackImpl(window, xpos, ypos);
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	getInstance().scrollCallbackImpl(window, xoffset, yoffset);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	getInstance().keyCallbackImpl(window, key, action);
}

void InputManager::mouseButtonCallbackImpl(GLFWwindow* window, int button, int action) //this is the actual implementation of the callback method
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//gl_world.camera_.StartMouseRotation(xpos, ypos);
		Graphics::getInstance().getGL_World().camera_.StartMouseRotation(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//gl_world.camera_.EndMouseRotation(xpos, ypos);
		Graphics::getInstance().getGL_World().camera_.EndMouseRotation(xpos, ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//gl_world.camera_.StartMousePan(xpos, ypos);
		Graphics::getInstance().getGL_World().camera_.StartMousePan(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//gl_world.camera_.EndMousePan(xpos, ypos);
		Graphics::getInstance().getGL_World().camera_.EndMousePan(xpos, ypos);
	}
}


void InputManager::cursorPositionCallbackImpl(GLFWwindow* window, double xpos, double ypos)
{
	//std::cout << xpos << " " << ypos << std::endl;

	Graphics::getInstance().getGL_World().camera_.ProcessMouseMotion(xpos, ypos);
}

void InputManager::scrollCallbackImpl(GLFWwindow* window, double xoffset, double yoffset)
{
	Graphics::getInstance().getGL_World().camera_.UpdateDolly(yoffset);
}

void InputManager::keyCallbackImpl(GLFWwindow* window, int key, int action)
{
	keyPress[key] = action;
}


bool InputManager::getKeyPressed(int key)
{
	return keyPress[key] == GLFW_PRESS || keyPress[key] == GLFW_REPEAT;
}