#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include "GL2/GL2_World.h"
#include "GL2/GL2_Light.h"
#include "GL2/GL_VIEW.h"

class CameraControl
{
public:
	// Singleton is accessed via getInstance()
	static CameraControl& getInstance()
	{
		// lazy singleton, instantiated on first use
		static CameraControl instance;
		return instance;
	}

	void setCamera(GL_VIEW* camera);
	void translateCamera(glm::vec3& d);

private:
	CameraControl(void) // private constructor necessary to allow only 1 instance
	{
		// 초기 위치 지정
		position = glm::vec3(0, 0, 1);
	}

	glm::vec3 position;

	GL_VIEW* camera;
};