/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "ConventionalMacros.h"
#include "DataStructure\Vector2D.h"

template<class T>
class Array2D
{
public:
	int i_res_ = 0;
	int j_res_ = 0;
	T* values_ = nullptr;

	Array2D()
	{}

	Array2D(const int& _i_res, const int& _j_res, const bool& init_flag = true)
	{
		init(_i_res, _j_res, init_flag);
	}

	void init(const int& _i_res, const int& _j_res, const bool& init_flag = true)
	{
		SAFE_DELETE_ARRAY(values_);

		i_res_ = _i_res;
		j_res_ = _j_res;

		values_ = new T[getNumAllElements()];
		
		if (init_flag == true)
			for (int p = 0; p < i_res_*j_res_; p++) values_[p] = T();
	}

	T& operator () (const int& i, const int& j) const
	{
		return values_[i + j * i_res_];
	}

	T& operator () (const Vector2D<int>& ij) const
	{
		return values_[ij.i_ + ij.j_ * i_res_];
	}

	const int& getNumAllElements() const
	{
		return i_res_ * j_res_;
	}
	
	void print() const
	{
		for(int j = j_res_-1; j >=0; j --)
		{
			for (int i = 0; i < i_res_; i ++)
			{
				std::cout << (*this)(i, j) << " " << std::flush;
			}

			std::cout << std::endl;
		}
	}
};
