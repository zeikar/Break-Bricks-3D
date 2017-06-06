#pragma once

#include <vector>
#include "DataStructure/Vector3D.h"

class Particle
{
public:
	Vector3D<float> pos_, vel_;
};

class Sphere
{
public:
	Vector3D<float> center_;
	float radius_;

	float getSignedDistance(const Vector3D<float>& pos)
	{
		return (pos - center_).getMagnitude() - radius_;
	}

	Vector3D<float> getNormal(const Vector3D<float>& pos)
	{
		return (pos - center_).getNormalized();
	}
};

class ParticleSystem
{
private:
	std::vector<Particle> particles_;
	Vector3D<float> startPos_;

public:
	bool isAbleToDelete = false;
	
	ParticleSystem(const Vector3D<float>& startPos)
	{
		startPos_ = startPos;

		particles_.resize(100);
		
		for (int p = 0; p < particles_.size(); ++p)
		{
			particles_[p].pos_.x_ = -0.5f + (float)rand() / (float)RAND_MAX + startPos_.x_;
			particles_[p].pos_.y_ = -0.1f + (float)rand() / (float)RAND_MAX * 0.2 + startPos_.y_;
			particles_[p].pos_.z_ = -0.5f + (float)rand() / (float)RAND_MAX + startPos_.z_;

			particles_[p].vel_ = Vector3D<float>(-0.5f + (float)rand() / (float)RAND_MAX, -1.0f, -0.5f + (float)rand() / (float)RAND_MAX);
		}
	}

	void advanceOneTimeStep(const float& dt)
	{
		const Vector3D<float> gravity = Vector3D<float>(0.0f, -0.5f, 0.0f);

		// Euler integration
		for (int p = 0; p < particles_.size(); ++p)
		{
			particles_[p].vel_ += gravity * dt;
			particles_[p].pos_ += particles_[p].vel_ * dt;
		}

		// Collision detection
		for (int p = 0; p < particles_.size(); ++p)
		{
			// 떨어지기 시작한 곳에서 2.0f 이상 떨어지면 이 파티클 삭제
			if (particles_[p].pos_.y_ < startPos_.y_ - 2.0f)
			{
				isAbleToDelete = true;
			}
		}
	}

	void renderParticles()
	{
		// old style rendering
		// draw particles

		glPointSize(5.0f);
		glBegin(GL_POINTS);

		glColor3f(1.0f, 0.0f, 0.0f);

		for (int p = 0; p < particles_.size(); p++)
		{
			glVertex3fv(particles_[p].pos_.values_);
		}

		glEnd();
	}
};