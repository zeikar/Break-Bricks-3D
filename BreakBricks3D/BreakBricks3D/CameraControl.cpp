#include "CameraControl.h"

void CameraControl::setCamera(GL_VIEW * camera)
{
	this->camera = camera;
	camera->SetCenterOfRotation(position);
}

void CameraControl::translateCamera(glm::vec3& d)
{
	camera->pan_ += d;

	d.z *= -1;
	position += d;

	camera->SetCenterOfRotation(position);
}

