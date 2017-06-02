/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <iostream>

#include "DataStructure/Vector3D.h"

template<class TT>
class Box3D
{
public:
	typedef Vector3D<TT> TTV;

	union
	{
		struct{ TT x_min_, y_min_, z_min_, x_max_, y_max_, z_max_; };
		struct{ TT i_start_, j_start_, k_start_, i_end_, j_end_, k_end_; };
	};

public:
	Box3D(void)
	{}

	Box3D(const Box3D<TT>& box_input)
		: x_min_(box_input.x_min_), y_min_(box_input.y_min_), z_min_(box_input.z_min_),
		  x_max_(box_input.x_max_), y_max_(box_input.y_max_), z_max_(box_input.z_max_)
	{}

	Box3D(const Vector3D<TT> min_input, const Vector3D<TT> max_input)
		: x_min_(min_input.x_), y_min_(min_input.y_), z_min_(min_input.z_), x_max_(max_input.x_), y_max_(max_input.y_), z_max_(max_input.z_)
	{}

	Box3D(const TT& x_min_input, const TT& y_min_input, const TT& z_min_input,
		   const TT& x_max_input, const TT& y_max_input, const TT& z_max_input)
		   : x_min_(x_min_input), y_min_(y_min_input), z_min_(z_min_input), x_max_(x_max_input), y_max_(y_max_input), z_max_(z_max_input)
	{}

	Box3D(const Vector3D<TT>& center, const TT& width)
	{
		initialize(center, width);
	}

	~Box3D(void)
	{}

	void initialize(const TT& x_min_input, const TT& y_min_input, const TT& z_min_input, const TT& x_max_input, const TT& y_max_input, const TT& z_max_input);
	void initialize(const Vector3D<TT>& center, const TT& width);
	void initialize(const Box3D<TT>& box);

	bool clampInside(TT& x, TT& y, TT& z);		// clamp (x,y,z) to be included by this box
	bool isInside(const TT& x, const TT& y, const TT& z);
	bool isInside(const Vector3D<TT>& pos) const;
	void extendMinAndMax(const TT& width);
	void extendMinAndMax(const Vector3D<TT>& width);
	Box3D<TT> getExtended(const TT& width) const;
	Box3D<TT> getExtended(const Vector3D<TT>& width) const;
	Box3D<TT> getZResized(const TT& z_min_input, const TT& z_max_input) const;
	void extendToInclude(const Vector3D<TT>& position);
	void enlargeMIN(const TT& x, const TT& y, const TT& z);
	void enlargeMAX(const TT& x, const TT& y, const TT& z);
	void enlargeToInclude(const Box3D<TT>& box_to_include);			// enlarge this box to include input box
	bool hasVolume() const;		// has volume

	void translate(const Vector3D<TT>& deviation);
	void scale(const TT s);
	void scale(const Vector3D<TT>& s);

	Box3D<TT> getTranslated(const Vector3D<TT>& deviation) const;

	Box3D<float> getAABB() const;

	Vector3D<TT> getEdgeLengths() const;
	Vector3D<TT> getMin() const;
	Vector3D<TT> getMax() const;

	TT getMaxEdgeLength() const;

	Vector3D<TT> getCenter() const;
	TT getMaxUnityScale() const;
	TT getMinUnityScale() const;

	TT getSignedDistance(const TTV& p) const;
	TTV getNormal(const TTV& p, const TT& epsilon) const;	
};

template<class TT> std::ostream& operator<<(std::ostream& output, const Box3D<TT>& box);
template<class TT> Box3D<TT> getIntersection(const Box3D<TT>& box_a, const Box3D<TT>& box_b);