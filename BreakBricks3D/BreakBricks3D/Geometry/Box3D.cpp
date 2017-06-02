/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "Box3D.h"
#include "ConventionalMacros.h"

template<class TT>
void Box3D<TT>::initialize(const TT& x_min_input, const TT& y_min_input, const TT& z_min_input, const TT& x_max_input, const TT& y_max_input, const TT& z_max_input)
{
	x_min_ = x_min_input;
	y_min_ = y_min_input;
	z_min_ = z_min_input;
	
	x_max_ = x_max_input;
	y_max_ = y_max_input;
	z_max_ = z_max_input;
}

template<class TT>
void Box3D<TT>::initialize(const Vector3D<TT>& center, const TT& width)
{
	const TT half_width = width / 2;

	initialize(center.x_ - half_width, center.y_ - half_width, center.z_ - half_width,
			   center.x_ + half_width, center.y_ + half_width, center.z_ + half_width);	
}

template<class TT>
void Box3D<TT>::initialize(const Box3D<TT>& box)
{
	x_min_ = box.x_min_;
	y_min_ = box.y_min_;
	z_min_ = box.z_min_;

	x_max_ = box.x_max_;
	y_max_ = box.y_max_;
	z_max_ = box.z_max_;
}

template<class TT>
Box3D<float> Box3D<TT>::getAABB() const
{
	return Box3D<float>(x_min_, y_min_, z_min_, x_max_, y_max_, z_max_);
}

template<class TT>
Box3D<TT> Box3D<TT>::getZResized(const TT& z_min_input, const TT& z_max_input) const
{
	return Box3D<TT>(x_min_, y_min_, z_min_input, x_max_, y_max_, z_max_input);
}

template<class TT>
bool Box3D<TT>::clampInside(TT& x, TT& y, TT& z)		// clamp (x,y,z) to be included by this box
{
	bool clamped = false;

	if (x < x_min_)
	{
		x = x_min_;
		clamped = true;
	}
	else if (x > x_max_)
	{
		x = x_max_;
		clamped = true;
	}

	if (y < y_min_)
	{
		y = y_min_;
		clamped = true;
	}
	else if (y > y_max_)
	{
		y = y_max_;
		clamped = true;
	}

	if (z < z_min_)
	{
		z = z_min_;
		clamped = true;
	}
	else if (z > z_max_)
	{
		z = z_max_;
		clamped = true;
	}

	return clamped;
}

template<class TT>
TT Box3D<TT>::getSignedDistance(const Vector3D<TT>& p) const // modified version of http://iquilezles.org/www/articles/distfunctions/distfunctions.htm
{
	const Vector3D<TT> box_center = Vector3D<TT>((x_min_ + x_max_)*(TT)0.5, (y_min_ + y_max_)*(TT)0.5, (z_min_ + z_max_)*(TT)0.5);
	const Vector3D<TT> box_half_edge = Vector3D<TT>((x_max_ - x_min_)*(TT)0.5, (y_max_ - y_min_)*(TT)0.5, (z_max_ - z_min_)*(TT)0.5);
	const Vector3D<TT> d = (p - box_center).getCompAbs() - box_half_edge;

	return d.getCompMax((TT)0).getMagnitude() + MIN2(MAX3(d.x_, d.y_, d.z_), 0.0);
}

template<class TT>
Vector3D<TT> Box3D<TT>::getNormal(const Vector3D<TT>& p, const TT& epsilon) const
{
	return TTV(getSignedDistance(p + TTV(epsilon, 0, 0)) - getSignedDistance(p + TTV(-epsilon, 0, 0)), getSignedDistance(p + TTV(0, epsilon, 0)) - getSignedDistance(p + TTV(0, -epsilon, 0)),
			   getSignedDistance(p + TTV(0, 0, epsilon)) - getSignedDistance(p + TTV(0, 0, -epsilon))).getSafeNormalized();
}

template<class TT>
bool Box3D<TT>::isInside(const TT& x, const TT& y, const TT& z)
{
	if (x < x_min_) return false;
	else if (x > x_max_) return false;

	if (y < y_min_) return false;
	else if (y > y_max_) return false;

	if (z < z_min_) return false;
	else if (z > z_max_) return false;

	return true;
}

template<class TT>
bool Box3D<TT>::isInside(const Vector3D<TT>& pos) const
{
	if (pos.x_ < x_min_) return false;
	else if (pos.x_ > x_max_) return false;

	if (pos.y_ < y_min_) return false;
	else if (pos.y_ > y_max_) return false;

	if (pos.z_ < z_min_) return false;
	else if (pos.z_ > z_max_) return false;

	return true;
}

template<class TT>
bool Box3D<TT>::hasVolume() const
{
	if (k_end_ < k_start_ || j_end_ < j_start_ || i_end_ < i_start_) return false;

	return true;
}

template<class TT>
void Box3D<TT>::extendMinAndMax(const TT& width)
{
	x_min_ -= width;
	y_min_ -= width;
	z_min_ -= width;

	x_max_ += width;
	y_max_ += width;
	z_max_ += width;
}

template<class TT>
void Box3D<TT>::extendMinAndMax(const Vector3D<TT>& width)
{
	x_min_ -= width.x_;
	y_min_ -= width.y_;
	z_min_ -= width.z_;

	x_max_ += width.x_;
	y_max_ += width.y_;
	z_max_ += width.z_;
}

template<class TT>
Box3D<TT> Box3D<TT>::getExtended(const TT& width) const
{
	Box3D<TT> box(*this);
	box.extendMinAndMax(width);
	return box;
}

template<class TT>
Box3D<TT> Box3D<TT>::getExtended(const Vector3D<TT>& width) const
{
	Box3D<TT> box(*this);
	box.extendMinAndMax(width);
	return box;
}

template<class TT>
void Box3D<TT>::extendToInclude(const Vector3D<TT>& position)
{
	x_min_ = MIN2(x_min_, (TT)position.x_);
	y_min_ = MIN2(y_min_, (TT)position.y_);
	z_min_ = MIN2(z_min_, (TT)position.z_);

	x_max_ = MAX2(x_max_, (TT)position.x_);
	y_max_ = MAX2(y_max_, (TT)position.y_);
	z_max_ = MAX2(z_max_, (TT)position.z_);
}

template<class TT>
void Box3D<TT>::enlargeMIN(const TT& x, const TT& y, const TT& z)
{
	x_min_ = MIN2(x_min_, x);
	y_min_ = MIN2(y_min_, y);
	z_min_ = MIN2(z_min_, z);
}

template<class TT>
void Box3D<TT>::enlargeMAX(const TT& x, const TT& y, const TT& z)
{
	x_max_ = MAX2(x_max_, x);
	y_max_ = MAX2(y_max_, y);
	z_max_ = MAX2(z_max_, z);
}


template<class TT>
void Box3D<TT>::enlargeToInclude(const Box3D<TT>& box_to_include)
{
	x_min_ = MIN2(x_min_, box_to_include.x_min_);
	y_min_ = MIN2(y_min_, box_to_include.y_min_);
	z_min_ = MIN2(z_min_, box_to_include.z_min_);

	x_max_ = MAX2(x_max_, box_to_include.x_max_);
	y_max_ = MAX2(y_max_, box_to_include.y_max_);
	z_max_ = MAX2(z_max_, box_to_include.z_max_);
}

template<class TT>
void Box3D<TT>::translate(const Vector3D<TT>& deviation)
{
	x_min_ += deviation.x_;
	y_min_ += deviation.y_;
	z_min_ += deviation.z_;

	x_max_ += deviation.x_;
	y_max_ += deviation.y_;
	z_max_ += deviation.z_;
}

template<class TT>
Box3D<TT> Box3D<TT>::getTranslated(const Vector3D<TT>& deviation) const
{
	return Box3D<TT>(x_min_ + deviation.x_, y_min_ + deviation.y_, z_min_ + deviation.z_, x_max_ + deviation.x_, y_max_ + deviation.y_, z_max_ + deviation.z_);
}

template<class TT>
void Box3D<TT>::scale(const TT s)
{
	x_min_ = x_min_ * s;
	y_min_ = y_min_ * s;
	z_min_ = z_min_ * s;

	x_max_ = x_max_ * s;
	y_max_ = y_max_ * s;
	z_max_ = z_max_ * s;
}

template<class TT>
void Box3D<TT>::scale(const Vector3D<TT>& s)
{
	x_min_ = x_min_ * s.x_;
	y_min_ = y_min_ * s.y_;
	z_min_ = z_min_ * s.z_;

	x_max_ = x_max_ * s.x_;
	y_max_ = y_max_ * s.y_;
	z_max_ = z_max_ * s.z_;
}

template<class TT>
Vector3D<TT> Box3D<TT>::getEdgeLengths() const
{
	return Vector3D<TT>(x_max_ - x_min_, y_max_ - y_min_, z_max_ - z_min_);
}

template<class TT>
TT Box3D<TT>::getMaxEdgeLength() const
{
	return MAX3(x_max_ - x_min_, y_max_ - y_min_, z_max_ - z_min_);
}

template<class TT>
Vector3D<TT> Box3D<TT>::getMin() const
{
	return Vector3D<TT>(x_min_, y_min_, z_min_);
}

template<class TT>
Vector3D<TT> Box3D<TT>::getMax() const
{
	return Vector3D<TT>(x_max_, y_max_, z_max_);
}

template<class TT>
Vector3D<TT> Box3D<TT>::getCenter() const
{
	return Vector3D<TT>((x_min_ + x_max_)*0.5f, (y_min_ + y_max_)*0.5f, (z_min_ + z_max_)*0.5f);
}

template<class TT>
TT Box3D<TT>::getMaxUnityScale() const
{
	float scale = MAX3(x_max_ - x_min_, y_max_ - y_min_, z_max_ - z_min_);

	if (scale != 0.0f)
	{
		scale = 1.0f / scale;

		return scale;
	}
	else return 1.0f;
}

template<class TT>
TT Box3D<TT>::getMinUnityScale() const
{
	float scale = MIN3(x_max_ - x_min_, y_max_ - y_min_, z_max_ - z_min_);

	if (scale != 0.0f)
	{
		scale = 1.0f / scale;

		return scale;
	}
	else return 1.0f;
}

template<class TT> std::ostream& operator<<(std::ostream& output, const Box3D<TT>& box)
{
	return output << "(" << box.x_min_ << "," << box.y_min_ << "," << box.z_min_ << ")x(" << box.x_max_ << "," << box.y_max_ << "," << box.z_max_ << ")";
}

template<class TT> Box3D<TT> getIntersection(const Box3D<TT>& box_a, const Box3D<TT>& box_b)
{
	Box3D<TT> box;

	box.i_start_ = MAX2(box_a.i_start_, box_b.i_start_);
	box.j_start_ = MAX2(box_a.j_start_, box_b.j_start_);
	box.k_start_ = MAX2(box_a.k_start_, box_b.k_start_);

	box.i_end_ = MIN2(box_a.i_end_, box_b.i_end_);
	box.j_end_ = MIN2(box_a.j_end_, box_b.j_end_);
	box.k_end_ = MIN2(box_a.k_end_, box_b.k_end_);

	return box;
}

template class Box3D<int>;
template class Box3D<float>;
template class Box3D<double>;
template bool Box3D<float>::isInside(const Vector3D<float>& pos) const;
template Box3D<int> getIntersection(const Box3D<int>& box_a, const Box3D<int>& box_b);
template Box3D<float> getIntersection(const Box3D<float>& box_a, const Box3D<float>& box_b);

template std::ostream& operator<<(std::ostream& output, const Box3D<float>& box);
template std::ostream& operator<<(std::ostream& output, const Box3D<int>& box);
