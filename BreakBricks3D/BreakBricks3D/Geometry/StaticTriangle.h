/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"
#include "DataStructure/LinkedArray.h"
#include "DataStructure/Vector3D.h"
#include "Geometry/Box3D.h"
#include "Geometry/PLANE.h"
#include <vector>

class StaticTriangle
{
public:
	TV v0_, v1_, v2_;

public:
	StaticTriangle()
	{}

	StaticTriangle(const TV& _v0, const TV& _v1, const TV& _v2)
		: v0_(_v0), v1_(_v1), v2_(_v2)
	{}

	TV	getClosestPointFromLine(const TV& location, const TV& x1, const TV& x2) const;
	TV	getBarycentricCoordinates(const TV& location) const;
	TV	getBarycentricCoordinates(const TV& location, const TV& x1, const TV& x2, const TV& x3) const;
	TV	getClosestPosition(const TV& location) const;
	T	getDistance(const TV& location) const;
	T	getArea() const;
	T	getAspectRatio() const;

	bool isInside(const TV& location) const;

	TV getNormal() const;

	TV getTangent(const int ix) const;

	bool checkOnTriangle(const TV& p, const float rad);

	void getXMinMax(T& x_min, T& x_max) const;
	void getYMinMax(T& y_min, T& y_max) const;
	void getZMinMax(T& z_min, T& z_max) const;

	Box3D<T> getAABB() const;

	TV project(const TV& p) const;
	void crossEdge(const TV& line0, const TV& line1, bool check[3], TV out[3]) const;
	void clampLine(TV& e0, TV& e1) const;

	template<class EE> const EE interpolate(const TV& q, const TV& p0, const TV& p1, const EE& v0, const EE& v1) const 
	{
		float dot = dotProduct(q - p0, p1 - p0) / dotProduct(p1 - p0, p1 - p0);
		dot = CLAMP(dot, 0.0f, 1.0f);
		return v0*(1.0f-dot) + v1*dot;
	}

	template<class EE> const EE interpolate(const TV& p, const EE arr[3]) const	
	{
		float dot = dotProduct(p - v0_, v1_ - v0_) / (dotProduct(v1_ - v0_, v1_ - v0_));
		dot = CLAMP(dot, 0.0f, 1.0f);

		TV v00 = v0_ + (v1_ - v0_) * dot;
		EE e00 = arr[0] * (1.0f - dot) + arr[1] * dot;


		dot = dotProduct(p - v0_, v2_ - v0_) / (dotProduct(v2_ - v0_, v2_ - v0_));
		dot = CLAMP(dot, 0.0f, 1.0f);

		TV v11 = v0_ + (v2_ - v0_) * dot;
		EE e11 = arr[0] * (1.0f - dot) + arr[2] * dot;

		
		dot = dotProduct(p - v00, v11 - v00) / dotProduct(v11 - v00, v11 - v00);
		dot = CLAMP(dot, 0.0f, 1.0f);


		EE result = e00*(1.0f - dot) + e11*dot;

		return result;
	}
};