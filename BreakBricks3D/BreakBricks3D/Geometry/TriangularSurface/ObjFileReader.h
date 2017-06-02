/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>
#include "DataStructure/Array1D.h"
#include "DataStructure/LinkedArray.h"
#include "DataStructure/Vector3D.h"
#include "GenericDefinitions.h"

class ObjFileReader
{
public:
	LinkedArray<TV> pos_stack_;
	LinkedArray<TV2> uv_stack_;
	LinkedArray<TV3> normal_stack_;

	LinkedArray<TV_INT> ix_stack_;
	LinkedArray<TV_INT> uv_ix_stack_;
	LinkedArray<TV_INT> nor_ix_stack_;

	//
	Array1D<TV3> posArray;
	Array1D<TV3> norArray;
	Array1D<TV2> texArray;

	Array1D<TV_INT> posIdxArray;
	Array1D<TV_INT> norIdxArray;
	Array1D<TV_INT> texIdxArray;

	float x_min_, x_max_, y_min_, y_max_, z_min_, z_max_;	// OOBB

	bool use_cout;
	bool cout_time_;

	ObjFileReader()
		: use_cout(false), cout_time_(false)
	{}

	void ReadOBJ(const char *filename);
	void WriteOBJ(const char* filename);

	const glm::vec3 GetCenterAABB() const;
	const float GetScaleAABB() const;
	const glm::vec3 GetScaleVecAABB() const;

	int GetTriangleNum() { return posIdxArray.num_elements_; }
	TV3 GetCenter();
	
	void GetPositionsFromTri(const int n, TV3 pos[3]);
	void GetTexCoordsFromTri(const int n, TV2 tex[3]);
	TV3 GetNormalFromTri(const int n);

	void GetPositionArray(Array1D<TV3>& pos_arr);
	void GetUVCoordArray(Array1D<TV2>& uv_arr, Array1D<TV3_INT>& tri_arr);
	void GetNormalArray(Array1D<TV3>& normal_arr);
	void GetTriangleArray(Array1D<TV3_INT>& tri_arr);

	void GetPositionArray(Array1D<float>& pos_arr);
	void GetNormalArray(Array1D<float>& nor_arr);
	void GetIndexArray(Array1D<unsigned short>& idx_arr);
};
