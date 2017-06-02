/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "StaticTriangle.h"

TV StaticTriangle::getClosestPosition(const TV& location) const
{
	static TV nor, p, weights;
	
	nor.assignCrossProduct(v1_ - v0_, v2_ - v0_);
	nor.normalize(); // normal unit of plane

	p.assignAMinusB(location, nor*dotProduct(nor, location - v0_));

	weights = getBarycentricCoordinates(p, v0_, v1_, v2_);// can be reused for velocity calculation

	// project closest point to the triangle if it's not already inside it
	if (weights.y_ < (T)0 && weights.z_ < (T)0)
	{
		return v0_; //return x1 
	}
	else if (weights.x_ < (T)0 && weights.z_ < (T)0)
	{
		return v1_; //return x2 
	}
	else if (weights.x_ < (T)0 && weights.y_ < (T)0)
	{
		return v2_; //return x3 
	}

	if (weights.x_ < (T)0) // Closest point is on edge x2--x3
	{
		return getClosestPointFromLine(p, v1_, v2_);
	}
	else if (weights.y_ < (T)0) // Closest point is on edge x1--x3
	{
		return getClosestPointFromLine(p, v0_, v2_);
	}
	else if (weights.z_ < (T)0) // Closest point is on edge x1--x2
	{
		return getClosestPointFromLine(p, v0_, v1_);
	}

	return p;
}

T  StaticTriangle::getDistance(const TV& location) const
{
	return (getClosestPosition(location) - location).getMagnitude();
}

T  StaticTriangle::getAspectRatio() const
{
	const T l0 = (v0_ - v1_).getMagnitude();
	const T l1 = (v0_ - v2_).getMagnitude();
	const T l2 = (v1_ - v2_).getMagnitude();

	const T s = (l0 + l1 + l2) * (T)0.5;
	const T aspect_ratio = l0 * l1 * l2 / ((T)8 * (s - l0) * (s - l1) * (s - l2));

	return aspect_ratio;
}

TV StaticTriangle::getBarycentricCoordinates(const TV& location) const
{
	return getBarycentricCoordinates(location, v0_, v1_, v2_);
}

TV StaticTriangle::getBarycentricCoordinates(const TV& location, const TV& v0_, const TV& v1_, const TV& v2_) const
{
	static TV u, v, w;
	u.assignAMinusB(v1_, v0_);
	v.assignAMinusB(v2_, v0_);
	w.assignAMinusB(location, v0_);
//	TV u = v1_ - v0_, v = v2_ - v0_, w = location - v0_;

	static T u_dot_u, v_dot_v, u_dot_v, u_dot_w, v_dot_w;
	u.assignDotProductTo(u, u_dot_u);
	v.assignDotProductTo(v, v_dot_v);
	u.assignDotProductTo(v, u_dot_v);
	u.assignDotProductTo(w, u_dot_w);
	v.assignDotProductTo(w, v_dot_w);

	static T denominator, one_over_denominator;
	
	denominator = u_dot_u*v_dot_v - POW2(u_dot_v);

	if (abs(denominator) > (T)1e-16)
	{
		one_over_denominator = 1 / denominator;
	}
	else
	{
		one_over_denominator = (T)1e16;
	}

	static T a, b;
	a = (v_dot_v*u_dot_w - u_dot_v*v_dot_w)*one_over_denominator;
	b = (u_dot_u*v_dot_w - u_dot_v*u_dot_w)*one_over_denominator;

	return TV(1 - a - b, a, b);
}

bool StaticTriangle::isInside(const TV& location) const
{
	TV3 w = getBarycentricCoordinates(location);

	if (w.x_ >= 0.0 && w.y_ >= 0.0 && w.z_ >= 0.0 &&
		w.x_ <= 1.0 && w.y_ <= 1.0 && w.z_ <= 1.0) {
		return true;
	}

	return false;
}

Box3D<T> StaticTriangle::getAABB() const
{
	Box3D<T> aabb;

	getXMinMax(aabb.x_min_, aabb.x_max_);
	getYMinMax(aabb.y_min_, aabb.y_max_);
	getZMinMax(aabb.z_min_, aabb.z_max_);

	return aabb;
}

void StaticTriangle::getXMinMax(T& x_min, T& x_max) const
{
	x_min = x_max = v0_.x_;

	x_min = MIN2(x_min, v1_.x_);
	x_max = MAX2(x_max, v1_.x_);

	x_min = MIN2(x_min, v2_.x_);
	x_max = MAX2(x_max, v2_.x_);
}

void StaticTriangle::getYMinMax(T& y_min, T& y_max) const
{
	y_min = y_max = v0_.y_;

	y_min = MIN2(y_min, v1_.y_);
	y_max = MAX2(y_max, v1_.y_);

	y_min = MIN2(y_min, v2_.y_);
	y_max = MAX2(y_max, v2_.y_);
}

void StaticTriangle::getZMinMax(T& z_min, T& z_max) const
{
	z_min = z_max = v0_.z_;

	z_min = MIN2(z_min, v1_.z_);
	z_max = MAX2(z_max, v1_.z_);

	z_min = MIN2(z_min, v2_.z_);
	z_max = MAX2(z_max, v2_.z_);
}

T StaticTriangle::getArea() const
{
	return ABS(crossProduct(v2_ - v0_, v1_ - v0_).getMagnitude()) * (T)0.5;

//	return (v0_.x_ * (v1_.y_ - v2_.z_) + v1_.x_ * (v2_.y_ - v0_.y_) + v2_.x_ * (v0_.y_ - v1_.y_)) * (T)0.5;		// 2D
}

TV StaticTriangle::getClosestPointFromLine(const TV& location, const TV& x1, const TV& x2) const
{
	T p = dotProduct(location - x1, x2 - x1) / dotProduct(x2 - x1, x2 - x1);

	if (p < (T)0)
	{
		return x1;
	}
	else if (p >(T)1)
	{
		return x2;
	}

	return x1 + (x2 - x1)*p;
}

TV StaticTriangle::getNormal() const
{
	return crossProduct(v1_ - v0_, v2_ - v0_).getNormalized();
}

TV StaticTriangle::getTangent(const int ix) const
{
	TV3 nor = crossProduct(v1_ - v0_, v2_ - v0_).getNormalized();
	TV3 tan;

	if (ix == 0) {
		tan = crossProduct(v1_ - v0_, nor);
	}
	else if (ix == 1) {
		tan = crossProduct(v2_ - v1_, nor);
	}
	else if (ix == 2) {
		tan = crossProduct(v2_ - v0_, nor);
	}

	return tan.getNormalized();
}

bool StaticTriangle::checkOnTriangle(const TV& p, const float rad)
{
	TV3 cen = (v0_ + v1_ + v2_) / 3.0f;

	TV3 n = crossProduct(v1_ - v0_, v2_ - v0_).getNormalized();
	float h = dotProduct(n, p - v0_);

	TV3 n0 = crossProduct(n, (v1_ - v0_).getNormalized()).getNormalized();
	if (dotProduct(v0_ - cen, n0) < 0.0f) {
		n0 *= -1.0f;
	}

	TV3 n1 = crossProduct(n, (v2_ - v1_).getNormalized()).getNormalized();
	if (dotProduct(v1_ - cen, n1) < 0.0f) {
		n1 *= -1.0f;
	}

	TV3 n2 = crossProduct(n, (v0_ - v2_).getNormalized()).getNormalized();
	if (dotProduct(v2_ - cen, n2) < 0.0f) {
		n2 *= -1.0f;
	}

	float d0 = dotProduct(n0, p - v0_);
	float d1 = dotProduct(n1, p - v1_);
	float d2 = dotProduct(n2, p - v2_);

	if (d0 <= 1e-04 && d1 <= 1e-04 && d2 <= 1e-04) {

		if (h >= -rad && h <= rad) {

			return true;

		}

	}

	return false;
}

TV StaticTriangle::project(const TV& p) const
{
	TV nor = getNormal();
	float d = dotProduct(nor, p - v0_);

	return p - nor * d;
}

void StaticTriangle::crossEdge(const TV& line0, const TV& line1, bool check[3], TV out[3]) const
{
	TV nor = getNormal();

	TV n0 = crossProduct(nor, v1_ - v0_).getNormalized();
	PLANE plane0(n0, v0_);

	TV n1 = crossProduct(nor, v2_ - v1_).getNormalized();
	PLANE plane1(n1, v1_);

	TV n2 = crossProduct(nor, v0_ - v2_).getNormalized();
	PLANE plane2(n2, v2_);

	check[0] = check[1] = check[2] = false;

	if (plane0.intersectRay(line0, line1, out[0]) == true)
	{
		T d = dotProduct(out[0] - v0_, v1_ - v0_) / dotProduct(v1_ - v0_, v1_ - v0_);
		if (d >= 0.0 && d <= 1.0) check[0] = true;
	}

	if (plane1.intersectRay(line0, line1, out[1]) == true)
	{
		T d = dotProduct(out[1] - v1_, v2_ - v1_) / dotProduct(v2_ - v1_, v2_ - v1_);
		if (d >= 0.0 && d <= 1.0) check[1] = true;
	}

	if (plane2.intersectRay(line0, line1, out[2]) == true)
	{
		T d = dotProduct(out[2] - v2_, v0_ - v2_) / dotProduct(v0_ - v2_, v0_ - v2_);
		if (d >= 0.0 && d <= 1.0) check[2] = true;
	}
}

void StaticTriangle::clampLine(TV& e0, TV& e1) const
{
	TV nor = getNormal();

	// Project to triangle plane
	//e0 = e0 - nor*dotProduct(e0 - v0_, nor);
	//e1 = e1 - nor*dotProduct(e1 - v0_, nor);

	TV dir = e1 - e0;

	TV w0 = getBarycentricCoordinates(e0);
	TV w1 = getBarycentricCoordinates(e1);

	bool check[3]; TV out[3];
	crossEdge(e0, e1, check, out);

	std::vector<TV> buff;

	if (w0.x_ >= 0.0 && w0.y_ >= 0.0 && w0.z_ >= 0.0 &&
		w0.x_ <= 1.0 && w0.y_ <= 1.0 && w0.z_ <= 1.0)
	{
		buff.push_back(e0);
	}

	if (w1.x_ >= 0.0 && w1.y_ >= 0.0 && w1.z_ >= 0.0 &&
		w1.x_ <= 1.0 && w1.y_ <= 1.0 && w1.z_ <= 1.0)
	{
		buff.push_back(e1);
	}

	if (buff.size() < 2 && check[0] == true)
	{
		buff.push_back(out[0]);
	}

	if (buff.size() < 2 && check[1] == true)
	{
		buff.push_back(out[1]);
	}

	if (buff.size() < 2 && check[2] == true)
	{
		buff.push_back(out[2]);
	}

	if (buff.size() == 2)
	{
		T dot = dotProduct(buff[1] - buff[0], e1 - e0);

		if (dot >= 0.0) {
			e0 = buff[0];
			e1 = buff[1];
		}
		else {
			e0 = buff[1];
			e1 = buff[0];
		}
	}
}