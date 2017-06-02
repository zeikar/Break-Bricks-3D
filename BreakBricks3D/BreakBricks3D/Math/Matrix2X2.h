/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <assert.h>

#include "GenericDefinitions.h"

class MATRIX_2X2
{
public:
	T x_[4];

public:
	MATRIX_2X2(void)
	{}

	MATRIX_2X2(const T x11,const T x21,const T x31,const T x12)
    {
        x_[0]=x11;x_[1]=x21;x_[2]=x31;x_[3]=x12;
    }

	~MATRIX_2X2(void)
	{}

public:
	T& operator()(const int& i,const int& j)
    {
		assert(i >= 1 && i <= 2);
		assert(j >= 1 && j <= 2);
		return x_[i-1+2*(j-1)]; // TODO : check
	}

	MATRIX_2X2 operator*(const MATRIX_2X2& A) const
    {
		return MATRIX_2X2(
			x_[0]*A.x_[0]+x_[2]*A.x_[1], x_[1]*A.x_[0]+x_[3]*A.x_[1],
			x_[0]*A.x_[2]+x_[2]*A.x_[3], x_[1]*A.x_[2]+x_[3]*A.x_[3]
		);
	}

	MATRIX_2X2 Transposed() const
    {
		return MATRIX_2X2(x_[0], x_[2], x_[1], x_[3]);
	}

	MATRIX_2X2 Inversed()
    {
		T determinant = x_[0]*x_[3] - x_[1]*x_[2];
		
		assert(determinant!=0);
		T s = 1/determinant;

		return MATRIX_2X2(x_[3]*s, -x_[1]*s, -x_[2]*s, x_[0]*s);
	}

   MATRIX_2X2 operator * (const T a) const
   {
		return MATRIX_2X2(a*x_[0],a*x_[1],a*x_[2],a*x_[3]);
   }

   static MATRIX_2X2 Identity()
   {
		return MATRIX_2X2(1, 0, 0, 1);
   }

   void operator += (const MATRIX_2X2& A)
   {
	   for(int i=0;i<4;i++) x_[i] += A.x_[i];
   }

   void operator *= (const T& a)
   {
	   for(int i=0;i<4;i++) x_[i] *= a;
   }

   void operator = (const MATRIX_2X2& A)
   {
	   for(int i=0;i<4;i++) x_[i] = A.x_[i];
   }
};

inline static TV2 operator * (const MATRIX_2X2& A, const TV2& v)
{
	return TV2(v.x_*A.x_[0]+v.y_*A.x_[1], v.x_*A.x_[2]+v.y_*A.x_[3]);
}

inline static MATRIX_2X2 operator * (const T s, const MATRIX_2X2& A)
{
	return MATRIX_2X2(s*A.x_[0], s*A.x_[1], s*A.x_[2], s*A.x_[3]);
}
