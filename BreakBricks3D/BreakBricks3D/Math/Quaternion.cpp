/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "Quaternion.h"

MATRIX_3X3 Quaternion::Matrix3X3() const // assumes quaternion is normalized; 18 mult and 12 add/sub
{
	T vx2=POW2(v_.x_), vy2=POW2(v_.y_), vz2=POW2(v_.z_), vxvy=v_.x_*v_.y_, vxvz=v_.x_*v_.z_, vyvz=v_.y_*v_.z_, svx=s_*v_.x_, svy=s_*v_.y_, svz=s_*v_.z_;
	return MATRIX_3X3(1-2*(vy2+vz2), 2*(vxvy+svz), 2*(vxvz-svy),
                        2*(vxvy-svz), 1-2*(vx2+vz2), 2*(vyvz+svx),
                        2*(vxvz+svy), 2*(vyvz-svx), 1-2*(vx2+vy2));
}

T Quaternion::Magnitude() const
{
	return sqrt(POW2(s_)+POW2(v_.x_)+POW2(v_.y_)+POW2(v_.z_));
}

void Quaternion::Normalize()
{
	T magnitude = Magnitude();
	assert(magnitude != 0);
		
	T r=1/magnitude;
	s_*=r;
	v_.x_*=r;
	v_.y_*=r;
	v_.z_*=r;
}

TV3 Quaternion::Rotate(const TV3& v) const // 32 mult and 26 add/sub
{
	Quaternion q=*this*Quaternion(v)*Inverse();
	return q.v_;
}

TV3 Quaternion::InversedRotate(const TV3& v) const
{
	Quaternion q = Inverse()*Quaternion(v)*(*this);
	return q.v_;
}

Quaternion Quaternion::Inverse() const
{
	return Quaternion(s_,-v_.x_,-v_.y_,-v_.z_);
}

Quaternion Quaternion::operator*(const Quaternion& q) const // 16 mult and 13 add/sub
{
	TV3 r=s_*q.v_+q.s_*v_+crossProduct(v_,q.v_);
	return Quaternion(s_*q.s_-dotProduct(v_,q.v_),r.x_,r.y_,r.z_);
}

Quaternion Quaternion::FromRotationVector(const TV3& v)
{
	const T magnitude = v.getMagnitude();
	if(magnitude <= (T)1e-8) return Quaternion();

	Quaternion q;	//TODO: find a way to use creator
	q.s_ = cos((T)0.5*magnitude);
	q.v_ = ((T)sin((T)0.5*magnitude)/magnitude)*v;
	return q;
}

Quaternion Quaternion::Slerp(const Quaternion& a_input, const Quaternion& b_input, T t)
{
	Quaternion a(a_input);
	Quaternion b(b_input);

	//T sina, sinat, sinaomt, angle;
	T fAlpha, fTheta, fBeta, fCosTheta, oosinTheta;

	fCosTheta = a.v_.x_*b.v_.x_ + a.v_.y_*b.v_.y_ + a.v_.z_*b.v_.z_ + a.s_*b.s_;

	if(fCosTheta < (T)0)
	{
		b.v_ *= (T)-1;
		b.s_ *= (T)-1;
		fCosTheta *= (T)-1;
	}

	fAlpha = t;
	fBeta = (T)1-fAlpha;

	if((T)1-fCosTheta > (T)0.001)
	{
		fTheta = acos(fCosTheta);
		oosinTheta = (T)1/sin(fTheta);
		fBeta = sin(fTheta*fBeta)*oosinTheta;
		fAlpha = sin(fTheta*fAlpha)*oosinTheta;
	}

	return Quaternion(fBeta*a.s_+fBeta*b.s_, fBeta*a.v_.x_+fBeta*b.v_.x_, fBeta*a.v_.y_+fBeta*b.v_.y_, fBeta*a.v_.z_+fBeta*b.v_.z_);
}

T Quaternion::GetAngle()
{
	return 2.0f * std::acos(s_);
}

TV3 Quaternion::GetAxis()
{
	T temp = std::sqrt(1 - s_*s_);
	if( temp <= FLT_MIN && temp >= -FLT_MIN)
		return TV3(1,0,0);

	return TV3(v_.x_ / temp, v_.y_ / temp,v_.z_ / temp);
}

float SIGNF(float x) {return (x >= 0.0f) ? +1.0f : -1.0f;}
float NORM(float a, float b, float c, float d) {return std::sqrt(a * a + b * b + c * c + d * d);}

Quaternion Quaternion::FromMatrix(float* mat)
{

	float q0;
	float q1;
	float q2;
	float q3;
	float r;

	float r11 = mat[0];
	float r21 = mat[1];
	float r31 = mat[2];

	float r12 = mat[3];
	float r22 = mat[4];
	float r32 = mat[5];

	float r13 = mat[6];
	float r23 = mat[7];
	float r33 = mat[8];

	q0 = ( r11 + r22 + r33 + 1.0f) / 4.0f;
	q1 = ( r11 - r22 - r33 + 1.0f) / 4.0f;
	q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if(q0 < 0.0f) q0 = 0.0f;
	if(q1 < 0.0f) q1 = 0.0f;
	if(q2 < 0.0f) q2 = 0.0f;
	if(q3 < 0.0f) q3 = 0.0f;
	q0 = std::sqrt(q0);
	q1 = std::sqrt(q1);
	q2 = std::sqrt(q2);
	q3 = std::sqrt(q3);
	if(q0 >= q1 && q0 >= q2 && q0 >= q3) 
	{
		q0 *= +1.0f;
		q1 *= SIGNF(r32 - r23);
		q2 *= SIGNF(r13 - r31);
		q3 *= SIGNF(r21 - r12);
	}else if(q1 >= q0 && q1 >= q2 && q1 >= q3) 
	{
		q0 *= SIGNF(r32 - r23);
		q1 *= +1.0f;
		q2 *= SIGNF(r21 + r12);
		q3 *= SIGNF(r13 + r31);
	}else if(q2 >= q0 && q2 >= q1 && q2 >= q3) 
	{
		q0 *= SIGNF(r13 - r31);
		q1 *= SIGNF(r21 + r12);
		q2 *= +1.0f;
		q3 *= SIGNF(r32 + r23);
	}else if(q3 >= q0 && q3 >= q1 && q3 >= q2) 
	{
		q0 *= SIGNF(r21 - r12);
		q1 *= SIGNF(r31 + r13);
		q2 *= SIGNF(r32 + r23);
		q3 *= +1.0f;
	}

	r = NORM(q0, q1, q2, q3);
	q0 /= r;
	q1 /= r;
	q2 /= r;
	q3 /= r;

	return Quaternion(q0, q1, q2, q3);
}
