/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "LinkedArrayBlock.h"
#include "Array1D.h"

template<class TT>
class LinkedArray
{
public:
	int new_block_size_;

	typedef LinkedArrayBlock<TT> T_BLOCK, *PT_BLOCK;

	T_BLOCK *head_;
	T_BLOCK *tail_;

	int num_blocks_;
	int num_elements_;

public:
	LinkedArray()
		: head_(nullptr), tail_(nullptr), new_block_size_(1000 / sizeof(TT)), num_blocks_(0), num_elements_(0)
	{
		initializeFirstBlock(new_block_size_);
	}

	LinkedArray(const int block_size_input)
		: head_(nullptr), tail_(nullptr), new_block_size_(block_size_input), num_blocks_(0), num_elements_(0)
	{
		initializeFirstBlock(new_block_size_);
	}

	~LinkedArray()
	{
		//deleteAll();
	}
	
	void initializeFirstBlock(const int new_block_size_input)
	{
		// make first block to avoid if(head_ == nullptr)
		T_BLOCK *new_block = new T_BLOCK(new_block_size_input, num_blocks_++, num_elements_);

		head_ = new_block;
		tail_ = new_block;
	}
	
	void reset()
	{		
		deleteAll();							//TODO: re-using the first block.
		initializeFirstBlock(new_block_size_);
	}
	
	void deleteAll()
	{
		for (T_BLOCK *itr = head_; itr != nullptr;)
		{
			T_BLOCK *temp = itr;

			itr = itr->next_;

			delete temp;
		}

		head_ = nullptr;
		tail_ = nullptr;
		num_blocks_ = 0;
		num_elements_ = 0;
	}
	
	TT& pushBack()	// push back
	{
		if (tail_->IsFull())		// add a new data block at the end of the list
		{
			T_BLOCK *new_block = new T_BLOCK(new_block_size_, num_blocks_++, num_elements_);

			tail_->next_ = new_block;
			tail_ = new_block;
		}

		num_elements_++;

		return tail_->PushBack();
	}
	
	TT popBack()
	{
		assert(false);

		// 	TT pop_value;
		// 
		// 	if (tail_ == nullptr)
		// 	{
		// 		assert(false);
		// 
		// 		return TT();
		// 	}
		// 	else if (tail_->IsEmpty() == false)
		// 	{
		// 
		// 	}

		return TT();
	}

	bool isEmpty() const
	{
		if (tail_ == nullptr) return true;
		else return false;
	}
	
	int countBlocks() const
	{
		int block_count = 0;
		T_BLOCK *current = head_;
		while (current != nullptr)
		{
			block_count++;

			current = current->next_;
		}

		return block_count;
	}

	int countElements() const
	{
		int element_count = 0;
		T_BLOCK *current = head_;
		while (current != nullptr)
		{
			element_count += current->num_elements_;

			current = current->next_;
		}

		return element_count;
	}
	
	void copyToArray(Array1D<TT>& to_array) const
	{
		to_array.init(num_elements_);

		for (T_BLOCK *itr_block = head_; itr_block != nullptr; itr_block = itr_block->next_)
		{
			itr_block->CopyToValues(to_array.values_);
		}
	}

	void copyToPartialArray(Array1D<TT>& to_array, const int offset) const
	{
		copyToPartialArray(to_array.values_, offset);
	}

	void copyToPartialArray(TT* to_array, const int offset) const
	{
		//	to_array.init(num_elements_);	// to_array should be initialized outside

		for (T_BLOCK *itr_block = head_; itr_block != nullptr; itr_block = itr_block->next_)
		{
			itr_block->CopyToValues(to_array, offset);
		}
	}
	
	int getSizeOfData() const
	{
		return num_elements_*sizeof(TT);
	}
	
	int getSizeOfType() const
	{
		return sizeof(TT);
	}
};