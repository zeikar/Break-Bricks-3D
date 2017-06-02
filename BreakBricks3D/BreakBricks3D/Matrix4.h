/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/


#pragma once

#include "Vector4.h"

/*
	Coded not for good performance, but for easy understanding.
*/

template<class T>
class Matrix4	// 4x4 matrix
{
public:
	T v_[4][4];	// [row][column] notation

	void setIdentity()
	{
		for(int r = 0; r < 4; r ++)
			for (int c = 0; c < 4; c++)
			{
				v_[r][c] = r == c ? (T)1 : (T)0;
			}
	}

	void setRow(const int& row, const T& v0, const T& v1, const T& v2, const T& v3)
	{
		v_[row][0] = v0;
		v_[row][1] = v1;
		v_[row][2] = v2;
		v_[row][3] = v3;
	}

	void multiply(const Vector4<T>& vec, Vector4<T>& out)
	{
		multiply(vec.v_, out.v_);
	}

	void multiply(const T vec[4], T out[4])
	{
		for (int r = 0; r < 4; ++r)
		{
			out[r] = (T)0;

			for (int c = 0; c < 4; ++c)
			{
				out[r] += v_[r][c] * vec[c];
			}
		}
	}

	void multiply(const T a[4][4], const T b[4][4])
	{
		for (int r = 0; r < 4; r++) { // row number of output
			for (int c = 0; c < 4; c++) { // column number of output
				v_[r][c] = 0;
				for (int z = 0; z < 4; z++) { // four elements are added for this output
					v_[r][c] += a[r][z] * b[z][c];
				}
			}
		}
	}

	friend std::ostream& operator<< (std::ostream& stream, const Matrix4<T>& mat)
	{
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				stream << mat.v_[r][c];

				if (c != 3) stream << " ";
				else stream << std::endl;
			}
		}

		return stream;
	}
};