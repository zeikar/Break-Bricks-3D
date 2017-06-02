/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "Box2D.h"

template<class TT>
bool Box2D<TT>::clampToBeInside(Vector2D<TT>& v)
{
	return clampToBeInside(v.x_, v.y_);
}

template<class TT>
bool Box2D<TT>::clampToBeInside(TT& x, TT& y)		// clamp (x,y) to be included by this box
{
	bool clamped = false;

	if(x < x_min_)
	{
		x = x_min_;
		clamped = true;
	}
	else if(x > x_max_)
	{
		x = x_max_;
		clamped = true;
	}

	if(y < y_min_)
	{
		y = y_min_;
		clamped = true;
	}
	else if(y > y_max_)
	{
		y = y_max_;
		clamped = true;
	}

	return clamped;
}

template<class TT>
bool Box2D<TT>::isInside(const TT& x, const TT& y)
{
	if(x < x_min_) return false;
	else if(x > x_max_) return false;

	if(y < y_min_) return false;
	else if(y > y_max_) return false;

	return true;
}

template<class TT>
bool Box2D<TT>::isInside(const Vector2D<TT>& pos) const
{
	if(pos.x_ < x_min_) return false;
	else if(pos.x_ > x_max_) return false;

	if(pos.y_ < y_min_) return false;
	else if(pos.y_ > y_max_) return false;

	return true;
}

template<class TT>
void Box2D<TT>::extendMinAndMax(const TT& width)
{
	x_min_ -= width;
	y_min_ -= width;
	x_max_ += width;
	y_max_ += width;
}

template<class TT>
Box2D<TT> Box2D<TT>::getExtended(const TT& width) const
{
	return Box2D<TT>(x_min_ - width, y_min_ - width, x_max_ + width, y_max_ + width);
}

template<class TT>
TT Box2D<TT>::getArea() const
{
	return (x_max_ - x_min_) * (y_max_ - y_min_);
}

template<class TT>
Vector2D<TT> Box2D<TT>::getMin() const
{
	return Vector2D<TT>(x_min_, y_min_);
}

template<class TT>
Vector2D<TT> Box2D<TT>::getMax() const
{
	return Vector2D<TT>(x_max_, y_max_);
}

template<class TT>
void Box2D<TT>::extendToInclude(const Vector2D<TT>& position)
{
	x_min_ = MIN2(x_min_, (TT)position.x_);
	y_min_ = MIN2(y_min_, (TT)position.y_);

	x_max_ = MAX2(x_max_, (TT)position.x_);
	y_max_ = MAX2(y_max_, (TT)position.y_);
}

template class Box2D<int>;
template class Box2D<float>;
template class Box2D<double>;
