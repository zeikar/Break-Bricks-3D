#pragma once

#include <vector>
#include "DataStructure/Vector3D.h"

typedef Vector3D<float> TV;

class Mass
{
public:
	Vector3D<float> pos_, vel_;

	bool is_fixed_ = false;
};

class Spring
{
public:
	Mass *m1 = nullptr;
	Mass *m2 = nullptr;

	float l0 = 0.0;

	void calc_l0()
	{
		l0 = (m1->pos_ - m2->pos_).getMagnitude();
	}
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


class MassSpringSystem
{
public:

	std::vector<Mass> mass_points_;
	std::vector<Spring> springs_;

	Sphere object;

	MassSpringSystem()
	{
		const int x_res = 10;
		const int y_res = 10;

		mass_points_.resize(x_res * y_res);

		for (int j = 0; j < x_res; j ++)
			for (int i = 0; i < y_res; i++)
			{
				mass_points_[i + x_res * j].pos_ = TV(i / (float)x_res, 0.8f, j / (float)y_res);

				if (i == 0) mass_points_[i + x_res * j].is_fixed_ = true;
			}

		// spring connections
		springs_.resize(9 * 10 + 10 * 9 + 9 * 9 * 2);

		int count = 0;
		for (int j = 0; j < 10; j++)
			for (int i = 0; i < 9; i++)
			{
				springs_[count].m1 = &mass_points_[i + 10 * j];
				springs_[count].m2 = &mass_points_[i + 1 + 10 * j];
				springs_[count].calc_l0();
				count++;
			}

		for (int j = 0; j < 9; j++)
			for (int i = 0; i < 10; i++)
			{
				springs_[count].m1 = &mass_points_[i + 10 * j];
				springs_[count].m2 = &mass_points_[i + 10 * (j + 1)];
				springs_[count].calc_l0();
				count++;
			}

		for (int j = 0; j < 9; j++)
			for (int i = 0; i < 9; i++)
			{
				springs_[count].m1 = &mass_points_[i + 10 * j];
				springs_[count].m2 = &mass_points_[i + 1 + 10 * (j + 1)];
				springs_[count].calc_l0();
				count++;

				springs_[count].m1 = &mass_points_[i + 10 * (j + 1)];
				springs_[count].m2 = &mass_points_[i + 1 + 10 * j];
				springs_[count].calc_l0();
				count++;
			}

		std::cout << springs_.size() << std::endl;
		assert(count == springs_.size());

		object.center_ = Vector3D<float>(0.5, 0.5, 0.5);
		object.radius_ = 0.2;
	}

	void advanceOneTimeStep(const float& dt)
	{
		const TV gravity = TV(0.0, -9.8, 0.0);

		// Euler integration
		for (int p = 0; p < mass_points_.size(); ++p)
		{
			mass_points_[p].vel_ += gravity * dt;

			if (mass_points_[p].is_fixed_ == true) {
				mass_points_[p].vel_ = TV(0, 0, 0);
				continue;
			}

			mass_points_[p].pos_ += mass_points_[p].vel_ * dt;
		}

		// Collision detection

		//for (int p = 0; p < mass_points_.size(); ++p)
		//{
		//	if (mass_points_[p].pos_.y_ < 0.0f && mass_points_[p].vel_.y_ < 0.0f)
		//	{
		//		mass_points_[p].vel_.y_ *= -0.4; // perfect restitution
		//	}

		//	if (object.getSignedDistance(mass_points_[p].pos_) < 0.0f && dotProduct(mass_points_[p].vel_, object.getNormal(mass_points_[p].pos_)) < 0.0f)
		//	{

		//		const float n_dot_v = dotProduct(mass_points_[p].vel_, object.getNormal(mass_points_[p].pos_));
		//		const Vector3D<float> normal_component = n_dot_v * object.getNormal(mass_points_[p].pos_);

		//		mass_points_[p].vel_ -= normal_component;
		//		//mass_points_[p].vel_ += 0.5f * normal_component;

		//	}

		//	if (mass_points_[p].is_fixed_ == true) {
		//		mass_points_[p].vel_ = TV(0, 0, 0);
		//		continue;
		//	}

		//}

		// add spring forces
		const float coeff = 100.0;
		for (int l = 0; l < springs_.size(); ++l)
		{
			const Spring &spr  = springs_[l];

			const float length = (spr.m1->pos_ - spr.m2->pos_).getMagnitude();
			const float dl = length - spr.l0;

			spr.m1->vel_ -= coeff * dl * dt * (spr.m1->pos_ - spr.m2->pos_).getNormalized();
			spr.m2->vel_ += coeff * dl * dt * (spr.m1->pos_ - spr.m2->pos_).getNormalized();
		}

		for (int p = 0; p < mass_points_.size(); ++p)
		{
			if (mass_points_[p].is_fixed_ == true) {
				mass_points_[p].vel_ = TV(0, 0, 0);
				continue;
			}

			mass_points_[p].pos_ += mass_points_[p].vel_ * dt;
		}

		//TODO: combine gravity force and spring forces



	}



};