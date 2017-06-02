/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>

class GL2_Material
{
public:
	glm::vec4 ambient_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 diffuse_    = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
	glm::vec4 specular_ = glm::vec4(0.2, 0.2, 0.2, 1.0);
	glm::vec4 emission_  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float shininess_ = 5.0;

	GL2_Material()
	{
	}

	~GL2_Material()
	{
	}

	void setRed()
	{
		ambient_ = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
		diffuse_ = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
		specular_ = glm::vec4(0.2, 0.2, 0.2, 1.0);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 5.0;
	}

	void setGold()
	{
		// http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
		ambient_ = glm::vec4(0.24725f, 0.2245f, 0.00645f, 1.0f);
		diffuse_ = glm::vec4(0.8f, 0.4f, 0.001f, 1.0f);
		specular_ = glm::vec4(0.797357f, 0.523991f, 0.00208006f, 1.0f);
		emission_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		shininess_ = 83.2;
	}
};
