/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

template<class T>
class Vector4
{
public:
	union
	{
		struct { T x_, y_, z_, w_; };
		T v_[4];
	};

	Vector4()
		: x_(T()), y_(T()), z_(T()), w_(T())
	{}

	Vector4(const T _x, const T _y, const T _z, const T _w)
		: x_(_x), y_(_y), z_(_z), w_(_w)
	{}

	void set(const T& v0, const T& v1, const T& v2, const T& v3)
	{
		v_[0] = v0;
		v_[1] = v1;
		v_[2] = v2;
		v_[3] = v3;
	}

	void operator *= (const T s)
	{
		x_ *= s;
		y_ *= s;
		z_ *= s;
		w_ *= s;
	}

	friend std::ostream& operator<< (std::ostream& stream, const Vector4<T>& vec)
	{
		stream << vec.x_ << " " << vec.y_ << " " << vec.z_ << " " << vec.w_;

		return stream;
	}
};