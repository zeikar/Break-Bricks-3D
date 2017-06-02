/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"
#include "ConventionalMacros.h"

template<class TT>
class Box2D
{
public:
	union
	{
		struct { TT x_min_, y_min_, x_max_, y_max_; };
		struct { TT i_start_, j_start_, i_end_, j_end_; };
		struct { TT min_[2], max_[2]; };
	};

	typedef Vector2D<TT> TTV;
	
public:
	Box2D(void)
	{}

	Box2D(const Box2D<TT>& box_input)
		: x_min_(box_input.x_min_), y_min_(box_input.y_min_), x_max_(box_input.x_max_), y_max_(box_input.y_max_)
	{}

	Box2D(const Vector2D<TT> min_input, const Vector2D<TT> max_input)
		: x_min_(min_input.x_), y_min_(min_input.y_), x_max_(max_input.x_), y_max_(max_input.y_)
	{}

	Box2D(const TT& x_min_input, const TT& y_min_input, const TT& x_max_input, const TT& y_max_input)
		: x_min_(x_min_input), y_min_(y_min_input), x_max_(x_max_input), y_max_(y_max_input)
	{}

	~Box2D(void)
	{}

	void init(const Box2D<TT>& b)
	{
		init(b.x_min_, b.y_min_, b.x_max_, b.y_max_);
	}

	void init(const TTV& min, const TTV& max)
	{
		init(min.x_, min.y_, max.x_, max.y_);
	}

	void init(const TT& x_min_input, const TT& y_min_input, const TT& x_max_input, const TT& y_max_input)
	{
		x_min_ = x_min_input;
		y_min_ = y_min_input;

		x_max_ = x_max_input;
		y_max_ = y_max_input;
	}

	bool clampToBeInside(Vector2D<TT>& v);
	bool clampToBeInside(TT& x, TT& y);		// clamp (x,y) to be included by this box
	bool isInside(const TT& x, const TT& y);
	bool isInside(const Vector2D<TT>& pos) const;
	void extendMinAndMax(const TT& width);
	void extendToInclude(const Vector2D<TT>& position);
	Vector2D<TT> getMin() const;
	Vector2D<TT> getMax() const;

	Box2D<TT> getExtended(const TT& width) const;

	TT getArea() const;

	TT getEdgeLength(const int& d) const
	{
		assert(max_[d] - min_[d] >= 0);

		return max_[d] - min_[d];
	}

	TTV getEdgeLengths() const
	{
		return TTV(getEdgeLength(0), getEdgeLength(1));
	}

	TT getMaxEdgeLength() const
	{
		return getEdgeLengths().getComponentWiseMax();
	}

	TTV getLeftBottom() const
	{
		return getMin();
	}

	TTV getRightBottom() const
	{
		return TTV(x_max_, y_min_);
	}

	TTV getRightUp() const
	{
		return TTV(x_max_, y_max_);
	}

	TTV getLeftUp() const
	{
		return TTV(x_min_, y_max_);
	}

	TTV getCenter() const
	{
		return (getMin() + getMax()) * (TT)0.5;
	}

	TT getAspectRatio() const
	{
		return getEdgeLength(0) / getEdgeLength(1);
	}

	Box2D<TT> getBoundingSquare() const
	{
		const TT longer_edge = getMaxEdgeLength();

		return Box2D<TT>(x_min_, y_min_, x_min_ + longer_edge, y_min_ + longer_edge);
	}

	void operator = (const Box2D<TT>& b)
	{
		init(b);
	}
};

template<class TT> std::ostream& operator<<(std::ostream& output, const Box2D<TT>& box)
{
	return output << "(" << box.x_min_ << "," << box.y_min_ << ")x(" << box.x_max_ << "," << box.y_max_ << ")";
}
