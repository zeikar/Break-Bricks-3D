/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include "GenericDefinitions.h"
#include "StaticTriangularSurface.h"

class MeshSimplification
{
public:
	T min_edge_length_;
	T kappa_;

	StaticTriangularSurface &surface_;

	Array1D<int> triangles_flag_;
	Array1D<int> vertices_flag_;

// 	MeshSimplification()
// 		: min_edge_length_((T)0), kappa_(1e8)
// 	{}

	MeshSimplification(const T min_edge_length_input, const T kappa_threshould_input, StaticTriangularSurface& surface_input)
		: min_edge_length_(min_edge_length_input), kappa_(kappa_threshould_input), surface_(surface_input)
	{}

	void update();

	int removeShortEdges(const T& min_edge_length, const T kappa);
	bool doesAdjTrisContact(const int& v0_ix, const int& v1_ix, const int& v2_except, const int& v3_except, const int& tri0_ix_ignore, const int& tri1_ix_ignore) const;
	void tagAdjTriangles(const int& v_ix, const int& tri_flag, const int recursive_count);
	void removeEdge(const int& tri_ix0, const int& tri_ix1, const int& edge_ix0, const int& edge_ix1, const int& v_ix0, const int& v_ix1);
	void removeEdgeSharingTriangles(const int &tri_ix0, const int & tri_ix1, const int& edge_ix0, const int& edge_ix1);
	bool containInvalidAdjTriangles(const int& v_ix);
};