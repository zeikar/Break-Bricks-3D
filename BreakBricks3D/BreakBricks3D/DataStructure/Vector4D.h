/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

template<class TT>
class Vector4D
{
public:
	TT x_, y_, z_, w_;

	Vector4D()
		: x_(TT()), y_(TT()), z_(TT()), w_(TT())
	{}

	Vector4D(const TT _x, const TT _y, const TT _z, const TT _w)
		: x_(_x), y_(_y), z_(_z), w_(_w)
	{}

	void operator *= (const TT s)
	{
		x_ *= s;
		y_ *= s;
		z_ *= s;
		w_ *= s;
	}

	friend std::ostream& operator<< (std::ostream& stream, const Vector4D<TT>& vec)
	{
		stream << vec.x_ << " " << vec.y_ << " " << vec.z_ << " "<< vec.w_;

		return stream;
	}
};