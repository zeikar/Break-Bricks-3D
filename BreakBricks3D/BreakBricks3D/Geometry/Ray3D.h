/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"

class Ray3D
{
public:
	TV p_start_;		// this is the start point when t_start_ = 0, otherwise start_point_ = p_start_ + direction * t_start_;
	TV direction_;		// direction_, normalize this if possible.
	T t_start_, t_end_; // actual start point is getStart() due when t_start_ is not 0.

public:
	Ray3D()
		: p_start_(TV()), direction_(TV()), t_start_((T)0), t_end_((T)0)
	{}

	Ray3D(const Ray3D& _ray)
		: p_start_(_ray.p_start_), direction_(_ray.direction_), t_start_(_ray.t_start_), t_end_(_ray.t_end_)
	{}

	Ray3D(const TV& _p_start, const TV& _dir, const T& _t_start, const T& _t_end)
		: p_start_(_p_start), direction_(_dir), t_start_(_t_start), t_end_(_t_end)
	{}
	
	Ray3D(const TV& _origin, const TV& _direction, const T& _t_end)
		: p_start_(_origin), direction_(_direction), t_start_((T)0), t_end_(_t_end)
	{}

	Ray3D(const TV& _start, const TV& _end)
//		: p_start_(_start), t_end_((_end - _start).getMagnitude()), direction_((_end-_start)/t_end_)
	{
		p_start_ = _start;
		t_start_ = (T)0;
		t_end_ = (_end - _start).getMagnitude();

		assert(t_end_ > 0);

		direction_ = (_end - _start) / t_end_;
	}

	void initialize(const TV& _p_start, const TV& _dir, const T& _t_start, const T& _t_end)
	{
		p_start_ = _p_start;
		direction_ = _dir;
		t_start_ = _t_start;
		t_end_ = _t_end;
	}

	T	 getSphereIntersection(const TV& center, const T& radius) const;
	TV	 getPosition(const T& t) const;
	bool checkTriangleIntersection(const TV& tri_v0, const TV& tri_v1, const TV& tri_v2, const Ray3D& segment) const;

	TV getStart() const;
	TV getEnd() const;
};
