#include "GameObject.h"
#include "Graphics.h"

GameObject::GameObject()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

GameObject::~GameObject()
{
}

void GameObject::readOBJ(const char* fileName)
{
	surface.readObj(fileName, true, true);

	gl_obj.initPhongSurface(surface);

	setMaterial();
}

void GameObject::setMaterial(int matType)
{
	gl_obj.mat_.setMaterial(matType);
}

void GameObject::render()
{
	if (isActive == false)
	{
		return;
	}

	//gl_obj.drawPhongSurface();
	gl_obj.applyLighting(Graphics::getInstance().getLight());
	gl_obj.drawWithShader(Graphics::getInstance().getGL_World().shaders_);
}

void GameObject::translate(const glm::vec3& direction)
{
	position += direction;

	surface.translate(TV(direction));

	updateShader();
}

glm::vec3 & GameObject::getScale()
{
	return scale;
}

void GameObject::setScale(const glm::vec3 & s)
{
	scale.x *= s.x;
	scale.y *= s.y;
	scale.z *= s.z;

	surface.scale(s.x, s.y, s.z, true);

	updateShader();
}

void GameObject::updateShader()
{
	// don't need to update if there is no change
	gl_obj.updatePhongSurface(surface);
}

bool GameObject::getActive()
{
	return isActive;
}

void GameObject::setActive(bool active)
{
	isActive = active;
}

glm::vec3 & GameObject::getPosition()
{	
	return position;
}
