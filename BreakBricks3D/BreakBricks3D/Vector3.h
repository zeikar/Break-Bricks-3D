/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once
#include <iostream>

template<class T>
class Vector3
{
public:
	union {
		struct { T x_, y_, z_; };
		T v_[3];
	};

	Vector3(const T& _x, const T& _y, const T& _z)
		: x_(_x), y_(_y), z_(_z)
	{}

	void print() const
	{
		std::cout << x_ << " " << y_ << " " << z_ << std::flush;
	}
};
