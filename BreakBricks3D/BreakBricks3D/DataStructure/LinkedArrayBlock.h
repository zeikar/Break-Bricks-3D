/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "ConventionalMacros.h"
#include <assert.h>

template<class TT>
class LinkedArrayBlock
{
public:
	int block_id_;					// # of this block
	int i_start_;					// global index

	int size_;						// memory size
	int num_elements_;				// number of added elements

	TT *values_;

	LinkedArrayBlock *prev_;
	LinkedArrayBlock *next_;

public:
	LinkedArrayBlock(const int& size_input, const int& block_id, const int& i_start)
		: block_id_(block_id), i_start_(i_start), size_(size_input), num_elements_(0), next_(nullptr), prev_(nullptr)
	{
		values_ = new TT[size_];
	}
	
	~LinkedArrayBlock()
	{
		SAFE_DELETE_ARRAY(values_);
	}
	
	bool IsFull() const
	{
		if (num_elements_ == size_) return true;
		else return false;

		assert(num_elements_ <= size_);
	}

	bool IsEmpty() const
	{
		if (num_elements_ == 0) return true;
		else return false;

		assert(num_elements_ >= 0);
	}

	TT& PushBack()
	{
		assert(num_elements_ < size_);

		return values_[num_elements_++];
	}

	/*
	TT PopBack()
	{
	return values_[--num_elements_];
	}
	*/
	
	int GetSizeOfData() const
	{
		return num_elements_*sizeof(TT);
	}
	
	int GetSizeOfType() const
	{
		return sizeof(TT);
	}
	
	void CopyToValues(TT* to_array) const
	{
		memcpy(&to_array[i_start_], values_, GetSizeOfData());
	}
	
	void CopyToValues(TT* to_array, const int to_array_offset) const
	{
		memcpy(&to_array[i_start_ + to_array_offset], values_, GetSizeOfData());
	}
};
