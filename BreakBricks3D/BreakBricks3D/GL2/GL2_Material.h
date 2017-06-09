/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>

class GL2_Material
{
public:
	static const int MAT_SIZE = 6;

	const glm::vec4 ambientArr[MAT_SIZE] = {
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.24725f, 0.2245f, 0.00645f, 1.0f),
		glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f),
		glm::vec4(0.15f, 0.15f, 0.15f, 1.0f),
		glm::vec4(0.15f, 0.15f, 0.15f, 1.0f),
		glm::vec4(0.15f, 0.15f, 0.15f, 1.0f)
	};

	const glm::vec4 diffuseArr[MAT_SIZE] = {
		glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
		glm::vec4(0.8f, 0.4f, 0.001f, 1.0f),
		glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f),
		glm::vec4(1.0f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.1f, 0.1f, 1.0f, 1.0f),
		glm::vec4(0.1f, 1.0f, 0.1f, 1.0f)
	};

	const glm::vec4 specularArr[MAT_SIZE] = {
		glm::vec4(0.2, 0.2, 0.2, 1.0),
		glm::vec4(0.797357f, 0.523991f, 0.00208006f, 1.0f),
		glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)
	};

	const glm::vec4 emissionArr[MAT_SIZE] = {
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
};

	const float shininessArr[MAT_SIZE] = {
		5.0f,
		83.2f,
		5.0f,
		5.0f,
		5.0f,
		5.0f
	};

	static const int MAT_GOLD = 1, MAT_GRAY = 2, MAT_RED = 3, MAT_BLUE = 4, MAT_GREEN = 5;
	glm::vec4 ambient_, diffuse_, specular_, emission_;
	float shininess_;

	GL2_Material()
	{
		setMaterial(0);
	}

	~GL2_Material()
	{
	}

	void setMaterial(const int matType = 0)
	{
		ambient_ = ambientArr[matType];
		diffuse_ = diffuseArr[matType];
		specular_ = specularArr[matType];
		emission_ = emissionArr[matType];
		shininess_ = shininessArr[matType];
	}
};
