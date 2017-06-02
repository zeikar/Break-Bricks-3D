/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <GL/glew.h>
#include "GL_VIEW.h"
#include "GL2_ShaderProgram.h"

class GL2_World
{
public:
	GL_VIEW camera_;

	GL2_ShaderProgram shaders_;

	GL2_World()
	{
		camera_.SetPanScale(0.01f);
		camera_.SetDollyStartPosition(-1.5f);
		camera_.SetDollyScale(0.1f);
		camera_.SetTrackballScale(0.1f);
		camera_.SetCenterOfRotation(glm::vec3(0, 0, 0));

		//initShaders(); // do this after glfw is initialized
	}

	~GL2_World()
	{
	}

	void initShaders()
	{
		// do this after glfw is initialized
		shaders_.readShaders("GL2/shaders/vertex_shader.glsl", "GL2/shaders/fragment_shader.glsl");
	}

	void drawAxes()
	{
		glDisable(GL_LIGHTING);

		// xyz axes
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
		glEnd();
	}
};
