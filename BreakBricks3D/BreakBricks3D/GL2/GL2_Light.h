/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>

class GL2_Light
{
public:
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	glm::vec4 position_;

	GL2_Light()
	{
		ambient_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		diffuse_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		specular_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		position_ = glm::vec4(0.0f, 10.3f, 0.0f, 1.0f);
	}

	~GL2_Light()
	{
	}

};
