/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"

class Sphere2D
{
public:
	TV2 center_;
	T	radius_;

public:
	Sphere2D(const TV2& center_, const T& radius_)
		: center_(center_), radius_(radius_)
	{};

	~Sphere2D(void)
	{};

public:
	T getSignedDistance(const TV2& position) const
	{
		return (position - center_).getMagnitude() - radius_;
	}

	TV2 getNormal(const TV2& position) const
	{
		return position - center_;
	}

	TV2 getUnitNormal(const TV2& position) const
	{
		return (position - center_).getNormalized();
	}

	T getArea() const
	{
		return PI * radius_ * radius_;
	}
};

typedef Sphere2D Circle;