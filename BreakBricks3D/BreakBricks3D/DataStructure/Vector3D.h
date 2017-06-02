/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <math.h>
#include <iostream>
#include <ostream>
#include "ConventionalMacros.h"
#include <glm/glm.hpp>
#include "Vector2D.h"

template<class TT>
class Vector3D
{
public:
	union
	{
		struct{ TT x_, y_, z_; };
		struct{ TT v0_, v1_, v2_; };
		struct{	int i_, j_, k_;	};
		TT values_[3];
	};

public:
	Vector3D()
		: x_(0), y_(0), z_(0)
	{}

	Vector3D(const TT v[3])
		: x_(v[0]), y_(v[1]), z_(v[2])
	{}

	Vector3D(const TT& x_i, const TT& y_i, const TT& z_i)
		: x_(x_i), y_(y_i), z_(z_i)
	{}

	Vector3D(const glm::vec3& input)
		: x_(input.x), y_(input.y), z_(input.z)
	{}

	Vector3D(const glm::vec4& input)
		: x_(input.x), y_(input.y), z_(input.z)
	{}

	Vector3D(const Vector2D<TT>& v_2d, const TT& _z)
		: x_(v_2d.x_), y_(v_2d.y_), z_(_z)
	{}

	const TT& operator[](const int i) const
	{
		assert(0 <= i && i <= 3);

		return values_[i];
	}

	void operator -= (const Vector3D& v)
	{
		x_ -= v.x_;
		y_ -= v.y_;
		z_ -= v.z_;
	}

	void operator *= (const TT& s)
	{
		x_ *= s;
		y_ *= s;
		z_ *= s;
	}

	void operator /= (const TT& s)
	{
		x_ /= s;
		y_ /= s;
		z_ /= s;
	}

	void operator += (const Vector3D& v)
	{
		x_ += v.x_;
		y_ += v.y_;
		z_ += v.z_;
	}

	void operator = (const Vector3D& v)
	{
		x_ = v.x_;
		y_ = v.y_;
		z_ = v.z_;
	}

	Vector3D operator + (const Vector3D& v) const
	{
		return Vector3D(x_ + v.x_, y_ + v.y_, z_ + v.z_);
	}

	Vector3D operator - (const Vector3D& v) const
	{
		return Vector3D(x_ - v.x_, y_ - v.y_, z_ - v.z_);
	}

	Vector3D operator * (const TT& s) const
	{
		return Vector3D(s*x_, s*y_, s*z_);
	}

	Vector3D operator - () const
	{
		return Vector3D(-x_, -y_, -z_);
	}

	TT getMagnitude() const
	{
		return (TT)sqrt(x_*x_ + y_*y_ + z_*z_);
	}

	double getMagnitudeDouble() const
	{
		return sqrt((double)x_*(double)x_ + (double)y_*(double)y_ + (double)z_*(double)z_);
	}

	double getSqrMagnitudeDouble() const
	{
		return (double)x_*(double)x_ + (double)y_*(double)y_ + (double)z_*(double)z_;
	}

	TT getSqrMagnitude() const
	{
		return x_*x_ + y_*y_ + z_*z_;
	}

	Vector3D<TT> getSafeNormalized() const
	{
		const TT magnitude = getMagnitude();

		Vector3D<TT> ans(*this);

		if (magnitude > 1e-8)
		{
			const TT s = 1 / magnitude;

			ans.x_ *= s;
			ans.y_ *= s;
			ans.z_ *= s;
		}
		else// allows runtime debugging
		{
			ans.x_ = 0;
			ans.y_ = 0;
			ans.z_ = 1;
		}

		return ans;
	}

	void normalize()
	{
		const TT magnitude = getMagnitude();

		const TT s = 1 / magnitude;

		x_ *= s;
		y_ *= s;
		z_ *= s;

// 		if (magnitude > 1e-8)
// 		{
// 			const TT s = 1 / magnitude;
// 
// 			x_ *= s;
// 			y_ *= s;
// 			z_ *= s;
// 		}
//  		else// allows runtime debugging
//  		{
// 			x_ = 0;
// 			y_ = 0;
// 			z_ = 1;
 //			std::cout<<"In Normalize(), magnitude of a vector is too small"<<std::endl;
// 			exit(1);
// 		}
	}

	void normalizeDouble()
	{
		const double magnitude = getMagnitudeDouble();

		const TT s = (TT)((double)1 / magnitude);

		x_ *= s;
		y_ *= s;
		z_ *= s;

		//if (magnitude > 1e-8)
		//{
		//	const TT s = (TT)1 / magnitude;

		//	x_ *= s;
		//	y_ *= s;
		//	z_ *= s;
		//}
		//else// allows runtime debugging
		//{
		//	x_ = 0;
		//	y_ = 0;
		//	z_ = 1;
		//	std::cout << "In Normalize(), magnitude of a vector is too small" << std::endl;
		//	// 			exit(1);
		//}
	}

	Vector3D<TT> getNormalized() const
	{
		Vector3D<TT> normalized_vector(x_, y_, z_);

		normalized_vector.normalize();

		return normalized_vector;
	}

	Vector3D<TT> normalizedDouble() const
	{
		Vector3D<TT> normalized_vector(x_, y_, z_);

		normalized_vector.normalizeDouble();

		return normalized_vector;
	}

	void assign(const TT& x_input, const TT& y_input, const TT& z_input)
	{
		x_ = x_input;
		y_ = y_input;
		z_ = z_input;
	}

	void assignAMinusB(const Vector3D<TT>& A, const Vector3D<TT>& B)
	{
		x_ = A.x_;
		y_ = A.y_;
		z_ = A.z_;

		x_ -= B.x_;
		y_ -= B.y_;
		z_ -= B.z_;
	}

	void assignAPlusB(const Vector3D<TT>& A, const Vector3D<TT>& B)
	{
		x_ = A.x_;
		y_ = A.y_;
		z_ = A.z_;

		x_ += B.x_;
		y_ += B.y_;
		z_ += B.z_;
	}

	void assignCrossProduct(const Vector3D<TT>& v1, const Vector3D<TT>& v2)
	{
		x_ = v1.y_*v2.z_ - v1.z_*v2.y_;
		y_ = v1.z_*v2.x_ - v1.x_*v2.z_;
		z_ = v1.x_*v2.y_ - v1.y_*v2.x_;
	}

	void assignDotProductTo(const Vector3D<TT>& v2, TT& ans) const
	{
		ans = x_*v2.x_ + y_*v2.y_ + z_*v2.z_;
	}

	const bool hasElement(const TT element) const
	{
		if (x_ == element) return true;
		else if (y_ == element) return true;
		else if (z_ == element) return true;
		else return false;
	}

	const int getElementIndex(const TT element) const
	{
		if (x_ == element) return 0;
		else if (y_ == element) return 1;
		else if (z_ == element) return 2;
		else return -1;
	}

	void setComponentWiseMin(const Vector3D<TT>& v1)
	{
		x_ = MIN2(x_, v1.x_);
		y_ = MIN2(y_, v1.y_);
		z_ = MIN2(z_, v1.z_);
	}

	void setComponentWiseMax(const Vector3D<TT>& v1)
	{
		x_ = MAX2(x_, v1.x_);
		y_ = MAX2(y_, v1.y_);
		z_ = MAX2(z_, v1.z_);
	}

	Vector3D<TT> getReversedCW()
	{
		return Vector3D<TT>(x_, z_, y_);
	}

	void swapYZ()
	{
		const TT temp = y_;
		y_ = z_;
		z_ = temp;
	}

	Vector3D<TT> getCompAbs() const
	{
		return Vector3D<TT>(ABS(x_), ABS(y_), ABS(z_));
	}

	Vector3D<TT> getCompMax(const int& limit) const
	{
		return Vector3D<TT>(MAX2(x_, limit), MAX2(y_, limit), MAX2(z_, limit));
	}

	Vector3D<TT> getCompMin(const int& limit) const
	{
		return Vector3D<TT>(MIN2(x_, limit), MIN2(y_, limit), MIN2(z_, limit));
	}

	Vector3D<TT> getSwapedYZ()
	{	
		return Vector3D<TT>(x_, z_, y_);
	}

	TT getMax()
	{
		TT a, b, c;
		INCREASING_SORT3(x_, y_, z_, a, b, c);
		return c;
	}

	TT getMin()
	{
		TT a, b, c;
		INCREASING_SORT3(x_, y_, z_, a, b, c);
		return a;
	}

	TT getAbsMax()
	{
		TT a, b, c;
		INCREASING_SORT3(ABS(x_), ABS(y_), ABS(z_), a, b, c);
		return c;
	}

	TT getAbsMin()
	{
		TT a, b, c;
		INCREASING_SORT3(ABS(x_), ABS(y_), ABS(z_), a, b, c);
		return a;
	}

	friend std::ostream& operator<< (std::ostream& stream, const Vector3D<TT>& vec)
	{
		stream << vec.x_ << " " << vec.y_ << " " << vec.z_;

		return stream;
	}

	glm::vec3 getGLM() const
	{
		return glm::vec3(x_, y_, z_);
	}

	glm::vec4 getGLMV4() const
	{
		return glm::vec3(x_, y_, z_, (T)0);
	}

	glm::vec4 getGLMP4() const
	{
		return glm::vec3(x_, y_, z_, (T)1);
	}

	Vector3D<double> getDouble() const
	{
		return Vector3D<double>((double)x_, (double)y_, (double)z_);
	}

	Vector3D<float> getFloat() const
	{
		return Vector3D<float>((float)x_, (float)y_, (float)z_);
	}

	void projectOnPlane(const Vector3D<TT>& plane_unit_normal)	// project this vector on a plane.
	{
		const TT l = dotProduct(*this, plane_unit_normal);

		const Vector3D<TT> n_component = plane_unit_normal * l;

		(*this) -= n_component;

		normalizeDouble();
	}
};

template<class TT>
bool operator == (Vector3D<TT>& v1, Vector3D<TT>& v2)
{
	return (v1.x_ == v2.x_ && v1.y_ == v2.y_ && v1.z_ == v2.z_);
}

template<class TT>
bool operator != (Vector3D<TT>& v1, Vector3D<TT>& v2)
{
	return (v1.x_ != v2.x_ || v1.y_ != v2.y_ || v1.z_ != v2.z_);
}

template<class TT> static Vector3D<TT> crossProduct(const Vector3D<TT>& v1, const Vector3D<TT>& v2)
{
	return Vector3D<TT>(v1.y_*v2.z_ - v1.z_*v2.y_, v1.z_*v2.x_ - v1.x_*v2.z_, v1.x_*v2.y_ - v1.y_*v2.x_);
}

template<class TT> static TT dotProduct(const Vector3D<TT>& v1, const Vector3D<TT>& v2)
{
	return v1.x_*v2.x_ + v1.y_*v2.y_ + v1.z_*v2.z_;
}


template<class TT> static Vector3D<TT> operator * (const TT& s, const Vector3D<TT>& v2)
{
	return Vector3D<TT>(s*v2.x_, s*v2.y_, s*v2.z_);
}

template<class TT> static Vector3D<TT> operator * (const Vector3D<TT>& v2, const TT& s)
{
	return Vector3D<TT>(s*v2.x_, s*v2.y_, s*v2.z_);
}

template<class TT> static Vector3D<TT> operator / (const Vector3D<TT>& v2, const TT& s)
{
	return Vector3D<TT>(v2.x_/s, v2.y_/s, v2.z_/s);
}

template<class TT>
const TT static getAngle(const Vector3D<TT>& vi, const Vector3D<TT>& vj)
{
	const TT dot = dotProduct(vi, vj);

	return acos(dot / (vi.getMagnitude()*vj.getMagnitude()));
}
