/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <assert.h>
#include "GenericDefinitions.h"

class MATRIX_3X3
{
public:
	T x_[9];

public:
	MATRIX_3X3(void)
	{}

	MATRIX_3X3(const T x11,const T x21,const T x31,const T x12,const T x22,const T x32,const T x13,const T x23,const T x33)
    {
        x_[0]=x11;x_[1]=x21;x_[2]=x31;x_[3]=x12;x_[4]=x22;x_[5]=x32;x_[6]=x13;x_[7]=x23;x_[8]=x33;
    }

	~MATRIX_3X3(void)
	{}

public:
	T& operator()(const int& i,const int& j)
    {
		assert(i >= 1 && i <= 3);
		assert(j >= 1 && j <= 3);
		return x_[i-1+3*(j-1)];
	}

	MATRIX_3X3 operator*(const MATRIX_3X3& A) const // 27 mults, 18 adds
    {
		return MATRIX_3X3(
			x_[0]*A.x_[0]+x_[3]*A.x_[1]+x_[6]*A.x_[2], x_[1]*A.x_[0]+x_[4]*A.x_[1]+x_[7]*A.x_[2], x_[2]*A.x_[0]+x_[5]*A.x_[1]+x_[8]*A.x_[2],
			x_[0]*A.x_[3]+x_[3]*A.x_[4]+x_[6]*A.x_[5], x_[1]*A.x_[3]+x_[4]*A.x_[4]+x_[7]*A.x_[5], x_[2]*A.x_[3]+x_[5]*A.x_[4]+x_[8]*A.x_[5],
			x_[0]*A.x_[6]+x_[3]*A.x_[7]+x_[6]*A.x_[8], x_[1]*A.x_[6]+x_[4]*A.x_[7]+x_[7]*A.x_[8], x_[2]*A.x_[6]+x_[5]*A.x_[7]+x_[8]*A.x_[8]
		);
	}

	MATRIX_3X3 Transposed() const
    {
		return MATRIX_3X3(x_[0],x_[3],x_[6],x_[1],x_[4],x_[7],x_[2],x_[5],x_[8]);
	}

	MATRIX_3X3 Inversed()
    {
		T cofactor11=x_[4]*x_[8]-x_[7]*x_[5], cofactor12=x_[7]*x_[2]-x_[1]*x_[8], cofactor13=x_[1]*x_[5]-x_[4]*x_[2];
		T determinant=x_[0]*cofactor11 + x_[3]*cofactor12 + x_[6]*cofactor13;

		assert(determinant!=0);

		T s=1/determinant;
		return MATRIX_3X3(s*cofactor11, s*cofactor12, s*cofactor13,
			             s*(x_[6]*x_[5]-x_[3]*x_[8]), s*(x_[0]*x_[8]-x_[6]*x_[2]), s*(x_[3]*x_[2]-x_[0]*x_[5]),
						 s*(x_[3]*x_[7]-x_[6]*x_[4]), s*(x_[6]*x_[1]-x_[0]*x_[7]), s*(x_[0]*x_[4]-x_[3]*x_[1]));
	}

   MATRIX_3X3 operator * (const T a) const
   {
		return MATRIX_3X3(a*x_[0],a*x_[1],a*x_[2],a*x_[3],a*x_[4],a*x_[5],a*x_[6],a*x_[7],a*x_[8]);
   }

   static MATRIX_3X3 Identity()
   {
		return MATRIX_3X3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }

   void operator += (const MATRIX_3X3& A)
   {
	   for(int i=0;i<9;i++) x_[i] += A.x_[i];
   }

   void operator *= (const T& a)
   {
	   for(int i=0;i<9;i++) x_[i] *= a;
   }

   void operator = (const MATRIX_3X3& A)
   {
	   for(int i=0;i<9;i++) x_[i] = A.x_[i];
   }
};

inline TV3 operator * (const MATRIX_3X3& A, const TV3& v)
{
	return TV3(v.x_*A.x_[0]+v.y_*A.x_[1]+v.z_*A.x_[2], v.x_*A.x_[3]+v.y_*A.x_[4]+v.z_*A.x_[5], v.x_*A.x_[6]+v.y_*A.x_[7]+v.z_*A.x_[8]);
}

inline MATRIX_3X3 operator * (const T s, const MATRIX_3X3& A)
{
	return MATRIX_3X3(s*A.x_[0], s*A.x_[1], s*A.x_[2], s*A.x_[3], s*A.x_[4], s*A.x_[5], s*A.x_[6], s*A.x_[7], s*A.x_[8]);
}
