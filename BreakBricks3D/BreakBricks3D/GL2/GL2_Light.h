/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>

class GL2_Light
{
public:
	glm::vec4 ambient_ = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	glm::vec4 diffuse_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specular_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 position_ = glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);

	GL2_Light()
	{
	}

	~GL2_Light()
	{
	}

};
