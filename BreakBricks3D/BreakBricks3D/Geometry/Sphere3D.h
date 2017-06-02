/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"
#include "Box3D.h"

class Sphere3D
{
public:
	TV center_;
	T  radius_;

	Sphere3D(const TV& _center, const T& _radius)
		: center_(_center), radius_(_radius)
	{}

	T getSignedDistance(const TV& position) const
	{
		return (position - center_).getMagnitude() - radius_;
	}

	TV getNormal(const TV& position) const
	{
		return (position - center_).getNormalized();
	}

	Box3D<T> getAABB() const
	{
		return Box3D<T>(center_, radius_*2.0f);
	}
};