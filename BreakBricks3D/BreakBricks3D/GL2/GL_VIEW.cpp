/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "GL_VIEW.h"
#include <iostream>

void GL_VIEW::reset()
{
	rot_ = rot_incr_ = glm::angleAxis(glm::degrees(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	trackball_scale_ = 1.0f;
	dolly_scale_ = 0.01f;
	pan_scale_ = 0.01f;
}

void GL_VIEW::Resize(const int width, const int height, const float fov, const float znear, const float zfar)
{
	SetWindowSize(width, height);

	const float aspect = (float)width / (float)(height ? height : 1);

	SetProjection(fov, aspect, znear, zfar);
}

void GL_VIEW::SetWindowSize(const int& _w, const int & _h)
{
	width_ = _w;
	height_ = _h;
}

void GL_VIEW::StartMouseRotation(int x, int y)
{
	is_trackball_active_ = true;

	rot_incr_ = glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	x_ = x;
	y_ = y;

	dx_ = dy_ = 0;
}

void GL_VIEW::EndMouseRotation(int x, int y)
{
	dx_ = x - x_;
	dy_ = y - y_;

	if (dx_ != 0 || dy_ != 0) Update();

	is_trackball_active_ = false;
}

void GL_VIEW::StartMouseDolly(int x, int y)
{
	is_dolly_active_ = true;

	x_ = x;
	y_ = y;
	dx_ = dy_ = 0;
}

void GL_VIEW::EndMouseDolly(int x, int y)
{
	if (dx_ != 0 || dy_ != 0) Update();
	
	is_dolly_active_ = false;
	dx_ = dy_ = 0;
}

void GL_VIEW::StartMousePan(int x, int y)
{
	is_pan_active_ = true;
	
	x_ = x;	
	y_ = y;
	dx_ = dy_ = 0;
}

void GL_VIEW::EndMousePan(int x, int y)
{
	if (dx_ != 0 && dy_ != 0) Update();
	 
	is_pan_active_ = false;
	dx_ = dy_ = 0;
}

void GL_VIEW::ProcessMouseMotion(int x, int y)
{
	if (is_trackball_active_ || is_dolly_active_ || is_pan_active_)
	{
		dx_ = x - x_;
		dy_ = y - y_;		

		
		if (dx_ == 0 && dy_ == 0) return; // generate problems

		Update();

		// this stops rotating when mouse didn't move before button release
		x_ = x;
		y_ = y;
	}
}

void GL_VIEW::ContinueRotation()
{
	//simply increment the rotation
	if (!is_trackball_active_) rot_ = rot_incr_*rot_;
	else rot_incr_ = glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 GL_VIEW::get_arcball_vector(int x, int y)
{
	glm::vec3 P = glm::vec3((float)x / (float)width_ * 2 - 1.0f, (float)y / (float)height_ * 2 - 1.0f, 0.0f);

	P.y = -P.y;
	
	float OP_squared = P.x * P.x + P.y * P.y;
	
	if (OP_squared <= 1 * 1) P.z = sqrt(1 * 1 - OP_squared);	// Pythagoras
	else P = glm::normalize(P);									// nearest point

	return P;
}

void GL_VIEW::UpdateTrackball()
{
	// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
	const glm::vec3 va = get_arcball_vector(x_, y_);
	const glm::vec3 vb = get_arcball_vector(x_ + dx_, y_ + dy_);

	const float angle = glm::degrees(acos(glm::min(1.0f, glm::dot(va, vb)))) * trackball_scale_;

	const glm::vec3 axis_in_camera_coord = glm::normalize(glm::cross(va, vb));

	rot_incr_ = glm::angleAxis(angle, axis_in_camera_coord);		// note
	
	rot_ = rot_incr_*rot_;
}

void GL_VIEW::UpdatePan() 
{
	glm::vec3 v(dx_, -dy_, 0);

	pan_ += v * pan_scale_;
}

void GL_VIEW::UpdateDolly(const float dy)
{
	glm::vec3 v(0, 0, dy);

	dolly_ += v * dolly_scale_;
}

void GL_VIEW::Update() 
{
	if (is_trackball_active_)	UpdateTrackball();
	if (is_dolly_active_)		UpdateDolly(dy_);
	if (is_pan_active_)			UpdatePan();
}

/*
void EXAMINE_MANIPULATOR::applyTransform() 
{
	glTranslatef(_pan[0], _pan[1], _pan[2]);
	glTranslatef(_dolly[0], _dolly[1], _dolly[2]);
	glTranslatef(_centroid[0], _centroid[1], _centroid[2]);
	ApplyRotation(_r);
	glTranslatef(-_centroid[0], -_centroid[1], -_centroid[2]);
}
*/

const glm::mat4 GL_VIEW::GetWorldViewMatrix() const
{
	//concatenate all the transforms
	return projection_ * glm::translate(pan_) * glm::translate(dolly_) 
		   * glm::translate(rot_center) * glm::mat4_cast(rot_) * glm::translate(-rot_center);
}

glm::vec4 GL_VIEW::unproject(const glm::mat4& model_matrix, const glm::vec4& screen_space) const
{
	const glm::vec4 world_space = glm::inverse(GetWorldViewMatrix() * model_matrix) * screen_space;

	return world_space / world_space.w;
}

const glm::mat4 GL_VIEW::GetScreenViewMatrix() const
{
	// glm::vec3(0.0f, 0.0f, -1.0f) is a default back step vector
	return projection_ * glm::translate(glm::vec3(0.0f, 0.0f, -1.0f)) *glm::mat4_cast(rot_);
}

void GL_VIEW::SetProjection(const float fov, const float aspect, const float zNear, const float zFar)
{
	projection_ = glm::perspective(fov, aspect, zNear, zFar);	// radian? degree?

	/*const float asp = (float)width_ / (float)height_;
	projection_ = glm::ortho(-1.0f*asp, 1.0f*asp, -1.0f, 1.0f, -10.0f, 10.0f);*/
}

// Set the speed for the trackball.
void GL_VIEW::SetTrackballScale(float scale) 
{
	trackball_scale_ = scale;
}

void GL_VIEW::SetDollyScale(float scale) 
{
	dolly_scale_ = scale;
}

void GL_VIEW::SetDollyStartPosition(float pos) 
{
	dolly_.z = pos;
}

void GL_VIEW::SetPanScale(float scale) 
{
	pan_scale_ = scale;
}

void GL_VIEW::SetCenterOfRotation(const glm::vec3& c) 
{
	rot_center = c;
}
