/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "Ray3D.h"

TV Ray3D::getPosition(const T& t) const
{
	return p_start_ + direction_ * t; // don't use t_end or t_start instead of t
}

// http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-9-ray-triangle-intersection/m-ller-trumbore-algorithm/
bool Ray3D::checkTriangleIntersection(const TV& v0, const TV& v1, const TV& v2, const Ray3D& segment) const
{
	const TV edge1 = v1 - v0;
	const TV edge2 = v2 - v0;
	const TV pvec  = crossProduct(segment.direction_, edge2);
	const T	 det   = dotProduct(edge1, pvec);

	if (det == 0) return false;			// the direction is parallel to the triangle

	const T  invDet = 1.0f / det;

	const TV tvec = segment.p_start_ - v0;

	const T u = dotProduct(tvec, pvec) * invDet;

	if (u < 0 || u > 1) return false;

	const TV qvec = crossProduct(tvec, edge1);

	const T v = dotProduct(segment.direction_, qvec) * invDet;

	if (v < 0 || u + v > 1) return false;

	const T t = dotProduct(edge2, qvec) * invDet;

	if (t < 0 || t > segment.t_end_) return false;

	return true;
}

//Code for Ray Sphere Intersection :
//rayStart is camera position
//rayDirection is direction of the ray i.e. (s - e)
T Ray3D::getSphereIntersection(const TV& center, const T& radius) const
{
	T t1 = -1;
	T t2 = -1;
	T t = -1;

	//temporary == e-c
	TV temporary = p_start_ - center;

	T b = 2 * (dotProduct(direction_, temporary));
	T a = dotProduct(direction_, direction_);
	T c = dotProduct(temporary, temporary) - (radius * radius);
	T disc;
	disc = b*b - 4 * a*c;
	if (disc < 0){
		return -1;	// no intersection
	}
	else if (disc >= 0)	// two intersections
	{
		T discriminent = sqrt(disc);
		t1 = (-b + discriminent) / (2 * a);
		t2 = (-b - discriminent) / (2 * a);

		if (t1 >= 0 && t2 >= 0) return MIN2(t1, t2);
		else
		{
			if (t1 >= 0) return t1;
			if (t2 >= 0) return t2;
		}
	}
	else{		// one intersection
		T discriminent = sqrt(disc);

		t1 = (-b + discriminent) / (2 * a);

		if (t1 >= 0) return t1;
	}

	return -1;
}

TV Ray3D::getStart() const
{
	return getPosition(t_start_);
}

TV Ray3D::getEnd() const
{
	return getPosition(t_end_);
}