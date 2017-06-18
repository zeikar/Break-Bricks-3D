#pragma once
#include "GL2\GL2_Object.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void readOBJ(const char* fileName, const char * textureFileName = nullptr);
	void setMaterial(int matType = 0);
	const GL2_Material& getMaterial();
	void render();
	void setPosition(const glm::vec3& pos);
	void translate(const glm::vec3& direction);
	glm::vec3& getScale();
	void setScale(const glm::vec3& s);
	void updateShader();
	glm::vec3& getPosition();

protected:
	bool hasTexture = false;
	glm::vec3 position, scale;

	StaticTriangularSurface surface;
	GL2_Object gl_obj;


	void setTexture(const char* textureFileName);
};