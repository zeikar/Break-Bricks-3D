/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"
#include "Matrix3X3.h"

class Quaternion
{
public:
	T s_; // cos(theta/2)
	TV3 v_;

public:
	Quaternion(void)
		:s_(1),v_(0,0,0)
	{}

	Quaternion(const T& angle_input, const TV3& direction_input)
        :s_(cos((T)0.5*angle_input)),v_(direction_input)
    {
        v_.normalize();
		v_ *= sin((T)0.5*angle_input);
    }

	Quaternion(const T s,const T x,const T y,const T z)
        :s_(s),v_(x,y,z)
    {}

	Quaternion(const TV3& v_input) // makes a vector into a quaternion (note difference from From_Rotation_Vector!)
        :s_(0),v_(v_input)
    {}

	Quaternion(MATRIX_3X3& A) // matches A with a quaternion
    {
		T trace=1+A(1,1)+A(2,2)+A(3,3);// trace=4*cos^2(theta/2)
		if(trace > 1){s_=T(.5)*sqrt(trace);v_.x_=A(3,2)-A(2,3);v_.y_=A(1,3)-A(3,1);v_.z_=A(2,1)-A(1,2);v_*=T(.25)/s_;}
		else{int i=(A(1,1) > A(2,2)) ? 1:2;i=(A(i,i) > A(3,3)) ? i:3; // set i to be the index of the dominating diagonal term
        switch(i){
            case 1:v_.x_=T(.5)*sqrt(1+A(1,1)-A(2,2)-A(3,3));v_.y_=T(.25)*(A(2,1)+A(1,2))/v_.x_;v_.z_=T(.25)*(A(1,3)+A(3,1))/v_.x_;s_=T(.25)*(A(3,2)-A(2,3))/v_.x_;break;
            case 2:v_.y_=T(.5)*sqrt(1-A(1,1)+A(2,2)-A(3,3));v_.x_=T(.25)*(A(2,1)+A(1,2))/v_.y_;v_.z_=T(.25)*(A(3,2)+A(2,3))/v_.y_;s_=T(.25)*(A(1,3)-A(3,1))/v_.y_;break;
            case 3:v_.z_=T(.5)*sqrt(1-A(1,1)-A(2,2)+A(3,3));v_.x_=T(.25)*(A(1,3)+A(3,1))/v_.z_;v_.y_=T(.25)*(A(3,2)+A(2,3))/v_.z_;s_=T(.25)*(A(2,1)-A(1,2))/v_.z_;break;}}
	}

	~Quaternion(void)
	{}

public:
	MATRIX_3X3 Matrix3X3() const; // assumes quaternion is normalized; 18 mult and 12 add/sub

	T Magnitude() const;

	void Normalize();

	TV3 Rotate(const TV3& v) const; // 32 mult and 26 add/sub

	TV3 InversedRotate(const TV3& v) const;

	Quaternion Inverse() const;

	Quaternion operator*(const Quaternion& q) const; // 16 mult and 13 add/sub

	T GetAngle();
	TV3 GetAxis();

	void Initialize(const Quaternion& rot_input)
	{
		s_ = rot_input.s_;
		v_ = rot_input.v_;
	}

	static Quaternion FromRotationVector(const TV3& v);

	static Quaternion Slerp(const Quaternion& a_input, const Quaternion& b_input, T t);

	static Quaternion FromMatrix(float* mat);
};

