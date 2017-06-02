/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "VertexOperations.h"

const TV& VertexOperations::GetPosition(const int& v_ix) const
{
	return vertex_positions_[v_ix];
}

const int VertexOperations::GetNumTriangles(const int& v_ix) const
{
	return start_ix_adj_tri_of_vertices_[v_ix + 1] - start_ix_adj_tri_of_vertices_[v_ix] + 1;
}
