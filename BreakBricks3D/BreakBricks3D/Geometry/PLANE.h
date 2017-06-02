/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"

class PLANE
{
public:
	TV n_, p_;	// use d_ instead of p_
	T d_;

	PLANE()
	{}

	PLANE(const TV& _n, const TV& _p)
		: n_(_n), p_(_p)
	{
		d_ = dotProduct(n_, p_);
	}

	const T getSignedDistance(const TV x) const
	{
		return dotProduct(x - p_, n_);
	}

	const T getDistance(const TV x) const
	{
		return ABS(dotProduct(x - p_, n_));
	}

	bool isCross(const TV& p0, const TV& p1) const 
	{
		float a = dotProduct(p0 - p_, n_);
		float b = dotProduct(p1 - p_, n_);

		if ((a >= 0.0 && b < 0.0) || (b >= 0.0 && a < 0.0)) return true;

		return false;
	}

	const TV intersectRay(const TV& p0, const TV& p1) const 
	{

		TV v = (p0 - p1).getNormalized();
		T t = -(dotProduct(p0, n_) - d_) / dotProduct(v, n_);

		return p0 + v*t;
	}

	const bool intersectRay(const TV& p0, const TV& p1, TV& out) const 
	{

		TV v = (p0 - p1).getNormalized();
		T t = -(dotProduct(p0, n_) - d_) / dotProduct(v, n_);

		out =  p0 + v*t;

		TV cen = n_ * d_;
		T dot0 = dotProduct(n_, p0 - cen);
		T dot1 = dotProduct(n_, p1 - cen);

		if ((dot0 >= 0.0 && dot1 < 0.0) || (dot1 >= 0.0 && dot0 < 0.0)) {
			return true;
		}

		return false;
	}
};