#include "GameObject.h"
#include "Graphics.h"

GameObject::GameObject()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::~GameObject()
{
}

void GameObject::readOBJ(const char* fileName)
{
	surface.readObj(fileName, true, true);

	gl_obj.initPhongSurface(surface);

	gl_obj.mat_.setGold();
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

void GameObject::setScale(const glm::vec3 & s)
{
	position.x *= s.x;
	position.y *= s.y;
	position.z *= s.z;

	surface.scale(s.x, s.y, s.z, true);

	updateShader();
}

void GameObject::updateShader()
{
	// don't need to update if there is no change
	gl_obj.updatePhongSurface(surface); 
}

void GameObject::setActive(bool active)
{
	isActive = active;
}
