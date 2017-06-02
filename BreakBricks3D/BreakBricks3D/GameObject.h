#pragma once
#include "GL2\GL2_Object.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void readOBJ(const char* fileName);
	void setMaterial();
	void render();
	void translate(const glm::vec3& direction);
	void setScale(const glm::vec3& s);
	void updateShader();
	void setActive(bool active);

private:
	bool isActive = true;
	glm::vec3 position, scale;

	StaticTriangularSurface surface;
	GL2_Object gl_obj;

};