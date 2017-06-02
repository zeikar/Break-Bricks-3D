#pragma once

#include <vector>
#include "DataStructure/Vector3D.h"

class Particle
{
public:
	Vector3D<float> pos_, vel_;

	//Particle(const Particle& p)
	//{
	//	pos_ = p.pos_;
	//	vel_ = p.vel_;
	//}
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
public:
	std::vector<Particle> particles_;

	Sphere object;

	ParticleSystem()
	{
		particles_.resize(1000);

		for (int p = 0; p < particles_.size(); ++p)
		{
			particles_[p].pos_.x_ = (float)rand() / (float)RAND_MAX *0.1 + 0.5f;
			particles_[p].pos_.y_ = (float)rand() / (float)RAND_MAX + 1.0f;
			particles_[p].pos_.z_ = (float)rand() / (float)RAND_MAX *0.1 + 0.5f;

			particles_[p].vel_ = Vector3D<float>(0.0f, 0.0f, 0.0f);
		}		

		object.center_ = Vector3D<float>(0.5, 0.5, 0.5);
		object.radius_ = 0.2;
	}

	void advanceOneTimeStep(const float& dt)
	{
		const TV gravity = TV(0.0, -9.8, 0.0);

		// Euler integration
		for (int p = 0; p < particles_.size(); ++p)
		{
			particles_[p].vel_ += gravity * dt;
			particles_[p].pos_ += particles_[p].vel_ * dt;
		}

		// Collision detection

		for (int p = 0; p < particles_.size(); ++p)
		{
			if (particles_[p].pos_.y_ < 0.0f && particles_[p].vel_.y_ < 0.0f)
			{
				particles_[p].vel_.y_ *= -0.4; // perfect restitution
			}

			if (object.getSignedDistance(particles_[p].pos_) < 0.0f && dotProduct(particles_[p].vel_, object.getNormal(particles_[p].pos_) ) < 0.0f)
			{

				const float n_dot_v = dotProduct(particles_[p].vel_, object.getNormal(particles_[p].pos_));
				const Vector3D<float> normal_component = n_dot_v * object.getNormal(particles_[p].pos_);

				particles_[p].vel_ -= normal_component;
				particles_[p].vel_ += 0.5f * normal_component;

			}

		}



	}

};
