#pragma once
#include "GL2\GL2_Object.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void readOBJ(const char* fileName, const char * textureFileName = nullptr);
	void setMaterial(int matType = 0);
	void setTexture(const char* textureFileName);
	void render();
	void translate(const glm::vec3& direction);
	glm::vec3& getScale();
	void setScale(const glm::vec3& s);
	void updateShader();
	bool getActive();
	void setActive(bool active);
	glm::vec3& getPosition();

protected:
	bool isActive = true, hasTexture = false;
	glm::vec3 position, scale;

	StaticTriangularSurface surface;
	GL2_Object gl_obj;

};