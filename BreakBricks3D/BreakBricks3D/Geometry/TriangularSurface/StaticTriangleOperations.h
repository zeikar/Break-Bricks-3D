/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <glm/glm.hpp>
#include <atomic>

#include "GenericDefinitions.h"
#include "DataStructure/LinkedArray.h"
#include "DataStructure/Vector3D.h"
#include "Geometry/Box3D.h"

class StaticTriangleOperations
{
private:
	Array1D<TV_INT> &triangles_;
	Array1D<TV>	 &vertex_positions_;

	Array1D<TV_INT> &edge_tri_ix_of_triangles_;		// adjacent triangles of triangles

public:
	StaticTriangleOperations(Array1D<TV>& _vertex_positions, Array1D<TV_INT>& _triangles, Array1D<TV_INT>& _edge_tri_ix_of_triangles)
		: vertex_positions_(_vertex_positions), triangles_(_triangles), edge_tri_ix_of_triangles_(_edge_tri_ix_of_triangles)
	{}

	TV  getCenter(const int& tri_index) const;
	TV  getNormal(const int& tri_index) const;
	TV  getNormalDouble(const int& tri_index) const;

	TV& getVertexPosition(const int& tri_ix, const int& v_ix) const;
	
	TV& getOppositeVertexPosOfEdgeTriangle(const int& tri_ix, const int& edge_number) const;
	TV& getOppositeVertexPos(const int& tri_ix, const int& vi, const int& vj) const;
	
	int getOppositeVertexIndexOfEdgeTriangle(const int& tri_ix, const int& edge_number) const;
	int getOppositeVertexIndex(const int& tri_ix, const int& vi, const int& vj) const;

	TV  getButterFlyEdgeVertex(const int& tri_ix, const int& edge_number) const;
	TV  getLinearEdgeVertex(const int& tri_ix, const int& edge_number) const;
	TV  getLoopEdgeVertex(const int& tri_ix, const int& edge_number) const;

	int getEdgeIndex(const int& tri_ix, const int& vi, const int& vj) const;
	int getEdgeIndexOfEdgeTriangle(const int& tri_ix, const int& edge_number) const;
	int getVertexIndex(const int& tri_ix, const int& v_ix) const;

	bool containVertex(const int& tri_ix, const int& vix) const;
	bool containsVertices(const int& tri_ix, const int& vi, const int& vj) const;
	bool hasDuplicatedEdgeTriangles(const int& tri_ix) const;
	bool shareVertex(const int& tri0_ix, const int& tri1_ix, const int& v0_except, const int& v1_except) const;

	T getSqrEdgeLength(const int& tri_ix, const int& edge_number) const;

	T getEdgeLength(const int& tri_ix, const int& edge_number) const
	{
		const TV_INT &vertices(triangles_[tri_ix]);

		//TODO: assert?

		return (vertex_positions_[vertices.values_[(edge_number + 1) % 3]] - vertex_positions_[vertices.values_[(edge_number + 2) % 3]]).getMagnitude();
	}

	int getShortEdgeIndex(const int& tri_ix, const T& sqr_min_edge_length) const;

	T getVoronoiArea(const int& tri_ix, const int& v0) const;
	void calculateMeanCurvatureHelper(const int& tri_ix, const int& v0, T& area_mix_sum, TV& mean_curvature_sum) const;

	Box3D<T> getAABB(const int& tri_ix) const;

	bool checkFlip(const int& tri_ix, const int& v_chage, const TV& new_pos) const;	// check if this triangle flips when v_ix = v_change moves to new_pos	

	bool isEdgeSharing(const int& tri0_ix, const int& tri1_ix) const;
};
