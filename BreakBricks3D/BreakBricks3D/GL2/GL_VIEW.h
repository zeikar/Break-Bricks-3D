/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

//#define GLM_FORCE_RADIANS	// check glm::angleAxis(glm::degrees(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class GL_VIEW
{
public:
	int width_, height_;

	int x_, y_;
	int dx_, dy_;

	// Variable used to determine if the manipulator is presently tracking the mouse
	bool is_trackball_active_;
	bool is_dolly_active_;
	bool is_pan_active_;

	glm::quat rot_;
	glm::quat rot_incr_;
	glm::vec3 rot_center;
	glm::vec3 pan_;
	glm::vec3 dolly_;

	glm::mat4 projection_;

	float trackball_scale_; //trackball scale
	float dolly_scale_;  //dolly scale
	float pan_scale_;   //pan scale

public:
	GL_VIEW()
		: is_trackball_active_(false), is_dolly_active_(false), is_pan_active_(false) 
	{
		reset();
	}

	void StartMouseRotation(int x, int y);
	void StartMouseDolly(int x, int y);
	void StartMousePan(int x, int y);
	void EndMouseRotation(int x, int y);
	void EndMouseDolly(int x, int y);
	void EndMousePan(int x, int y);
	void ProcessMouseMotion(int x, int y);

	void Update();
	void ContinueRotation();
	void UpdateTrackball();
	void UpdatePan();
	void UpdateDolly(const float dy_);

	const glm::mat4 GetWorldViewMatrix() const;
	const glm::mat4 GetScreenViewMatrix() const;

	void Resize(const int width, const int height, const float fov, const float znear, const float zfar);
	void reset();

	void SetTrackballScale(float scale);
	void SetDollyScale(float scale);
	void SetDollyStartPosition(float pos);
	void SetPanScale(float scale);
	void SetProjection(const float fov, const float aspect, const float zNear, const float zFar);
	void SetCenterOfRotation(const glm::vec3& c);
	void SetWindowSize(const int& _w, const int & _h);

	glm::vec3 get_arcball_vector(int x, int y);

	glm::vec4 unproject(const glm::mat4& mode_matrix, const glm::vec4& screen_space) const;
};
