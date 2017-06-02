/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include <math.h>

#include "StaticTriangleOperations.h"

TV StaticTriangleOperations::getCenter(const int& tri_index) const
{
	const TV_INT &vertex_indices(triangles_[tri_index]);

	const TV &v0(vertex_positions_[vertex_indices[0]]);
	const TV &v1(vertex_positions_[vertex_indices[1]]);
	const TV &v2(vertex_positions_[vertex_indices[2]]);

	return (v0 + v1 + v2) / 3.0f;
}

TV StaticTriangleOperations::getNormal(const int& tri_index) const 
{
	const TV_INT &vertex_indices(triangles_[tri_index]);

	const TV &v0(vertex_positions_[vertex_indices[0]]);
	const TV &v1(vertex_positions_[vertex_indices[1]]);
	const TV &v2(vertex_positions_[vertex_indices[2]]);

	return crossProduct(v1 - v0, v2 - v0).getNormalized();
}

TV StaticTriangleOperations::getNormalDouble(const int& tri_index) const
{
	const TV_INT &vertex_indices(triangles_[tri_index]);

	const TV &v0_float(vertex_positions_[vertex_indices[0]]);
	const TV &v1_float(vertex_positions_[vertex_indices[1]]);
	const TV &v2_float(vertex_positions_[vertex_indices[2]]);

	const Vector3D<double> v0_d(v0_float.v0_, v0_float.v1_, v0_float.v2_);
	const Vector3D<double> v1_d(v1_float.v0_, v1_float.v1_, v1_float.v2_);
	const Vector3D<double> v2_d(v2_float.v0_, v2_float.v1_, v2_float.v2_);

	const Vector3D<double> ans = crossProduct(v1_d - v0_d, v2_d - v0_d).getNormalized();

	return Vector3D<float>(ans.x_, ans.y_, ans.z_);
}

TV& StaticTriangleOperations::getVertexPosition(const int& tri_ix, const int& v_ix) const
{
	return vertex_positions_[triangles_[tri_ix][v_ix]];
}

TV& StaticTriangleOperations::getOppositeVertexPosOfEdgeTriangle(const int& tri_ix, const int& edge_number) const
{
	return getOppositeVertexPos(edge_tri_ix_of_triangles_[tri_ix][edge_number],triangles_[tri_ix][(edge_number + 1) % 3],triangles_[tri_ix][(edge_number + 2) % 3]);
}

TV& StaticTriangleOperations::getOppositeVertexPos(const int& tri_ix, const int& vi, const int& vj) const
{
	const int edge_number = getEdgeIndex(tri_ix, vi, vj);

	assert(edge_number != -1);

	return vertex_positions_[triangles_[tri_ix][edge_number]];
}

int StaticTriangleOperations::getOppositeVertexIndexOfEdgeTriangle(const int& tri_ix, const int& edge_number) const
{
	return getOppositeVertexIndex(edge_tri_ix_of_triangles_[tri_ix][edge_number], triangles_[tri_ix][(edge_number + 1) % 3], triangles_[tri_ix][(edge_number + 2) % 3]);
}

int StaticTriangleOperations::getOppositeVertexIndex(const int& tri_ix, const int& vi, const int& vj) const
{
	const int edge_number = getEdgeIndex(tri_ix, vi, vj);

	return triangles_[tri_ix][edge_number];
}

TV StaticTriangleOperations::getButterFlyEdgeVertex(const int& tri0_ix, const int& tri0_edge) const
{
	const T w = 1.0f / 16.0f;

	const int tri1_ix = edge_tri_ix_of_triangles_.values_[tri0_ix].values_[tri0_edge];
	const int tri1_edge = getEdgeIndexOfEdgeTriangle(tri0_ix, tri0_edge);

	TV pos;
	pos += (getVertexPosition(tri0_ix, (tri0_edge + 1) % 3) + getVertexPosition(tri0_ix, (tri0_edge + 2) % 3))*0.5f;
	pos += 2.0f*w*(getVertexPosition(tri0_ix, tri0_edge) + getVertexPosition(tri1_ix, tri1_edge));
	pos -= w*(getOppositeVertexPosOfEdgeTriangle(tri0_ix, (tri0_edge + 1) % 3) + getOppositeVertexPosOfEdgeTriangle(tri0_ix, (tri0_edge + 2) % 3));
	pos -= w*(getOppositeVertexPosOfEdgeTriangle(tri1_ix, (tri1_edge + 1) % 3) + getOppositeVertexPosOfEdgeTriangle(tri1_ix, (tri1_edge + 2) % 3));

	return pos;
}

TV StaticTriangleOperations::getLinearEdgeVertex(const int& tri0_ix, const int& tri0_edge) const
{
	const int tri1_ix = edge_tri_ix_of_triangles_[tri0_ix][tri0_edge];
	const int tri1_edge = getEdgeIndexOfEdgeTriangle(tri0_ix, tri0_edge);

	TV pos;
	pos += (getVertexPosition(tri0_ix, (tri0_edge + 1) % 3) + getVertexPosition(tri0_ix, (tri0_edge + 2) % 3))*0.5f;

	return pos;
}

TV StaticTriangleOperations::getLoopEdgeVertex(const int& tri0_ix, const int& tri0_edge) const
{
	const T w = 1.0f / 16.0f;

	const int tri1_ix = edge_tri_ix_of_triangles_.values_[tri0_ix].values_[tri0_edge];
	const int tri1_edge = getEdgeIndexOfEdgeTriangle(tri0_ix, tri0_edge);

	TV pos;
	pos += (getVertexPosition(tri0_ix, (tri0_edge + 1) % 3) + getVertexPosition(tri0_ix, (tri0_edge + 2) % 3))*0.375f;
	pos += (getVertexPosition(tri0_ix, tri0_edge) + getVertexPosition(tri1_ix, tri1_edge))*0.125f;

	return pos;
}

int StaticTriangleOperations::getEdgeIndex(const int& tri_ix, const int& vi, const int& vj) const
{
	const int v0_ix = triangles_[tri_ix].v0_;
	const int v1_ix = triangles_[tri_ix].v1_;
	const int v2_ix = triangles_[tri_ix].v2_;

	if ((v0_ix == vi && v1_ix == vj) || (v0_ix == vj && v1_ix == vi)) return 2;
	if ((v1_ix == vi && v2_ix == vj) || (v1_ix == vj && v2_ix == vi)) return 0;
	if ((v0_ix == vi && v2_ix == vj) || (v0_ix == vj && v2_ix == vi)) return 1;

	return -1;	// doesn't contain vi-vj edge.
}

int StaticTriangleOperations::getEdgeIndexOfEdgeTriangle(const int& tri_ix, const int& edge_number) const
{
	const int edge_tri_ix = edge_tri_ix_of_triangles_[tri_ix][edge_number];

	return getEdgeIndex(edge_tri_ix, triangles_[tri_ix][(edge_number + 1) % 3], triangles_[tri_ix][(edge_number + 2) % 3]);
}

bool StaticTriangleOperations::shareVertex(const int& tri0_ix, const int& tri1_ix, const int& v0_except, const int& v1_except) const
{
	const TV_INT tri0(triangles_[tri0_ix]);
	const TV_INT tri1(triangles_[tri1_ix]);

	for (int d0 = 0; d0 < 3; d0++)
		for (int d1 = 0; d1 < 3; d1++)
		if (tri0.values_[d0] == tri1.values_[d1])
		{
			if (tri0.values_[d0] == v0_except || tri0.values_[d0] == v1_except) continue;

			return true;
		}

	return false;
}

bool StaticTriangleOperations::containVertex(const int& tri_ix, const int& vix) const
{
	const TV_INT triangle(triangles_[tri_ix]);

	for (int d = 0; d < 3; d++)
	{
		if (triangle.values_[d] == vix) return true;
	}

	return false;
}

bool StaticTriangleOperations::containsVertices(const int& tri_ix, const int& vi, const int& vj) const
{
	const TV_INT &triangle(triangles_[tri_ix]);
	const int &v0(triangle.x_), &v1(triangle.y_), &v2(triangle.z_);

	if (vi == v0)
	{
		if (v1 == vj || v2 == vj) return true;
		return false;
	}
	else if (vi == v1)
	{
		if (v0 == vj || v2 == vj) return true;
		return false;
	}
	else if (vi == v2)
	{
		if (v0 == vj || v1 == vj) return true;
		return false;
	}
	else return false;
}

bool StaticTriangleOperations::hasDuplicatedEdgeTriangles(const int& tri_ix) const
{
	const TV_INT& edge_tris(edge_tri_ix_of_triangles_[tri_ix]);

	if (edge_tris.v0_ == edge_tris.v1_ || edge_tris.v0_ == edge_tris.v2_ || edge_tris.v1_ == edge_tris.v2_) return true;

	return false;
}

T StaticTriangleOperations::getSqrEdgeLength(const int& tri_ix, const int& edge_number) const
{
	const TV_INT &vertices(triangles_[tri_ix]);

	return (vertex_positions_[vertices.values_[(edge_number + 1) % 3]] - vertex_positions_[vertices.values_[(edge_number + 2) % 3]]).getSqrMagnitude();
}

int FindSmallestIndex(const T v[3])
{
	if (v[0] < v[1] && v[0] < v[2]) return 0;
	if (v[1] < v[0] && v[1] < v[2]) return 1;

	return 2;
}

int StaticTriangleOperations::getShortEdgeIndex(const int& tri_ix, const T& sqr_min_edge_length) const
{
	const TV_INT &vertices(triangles_[tri_ix]);

	T sqr_l[3];
	sqr_l[0] = (vertex_positions_[vertices.v1_] - vertex_positions_[vertices.v2_]).getSqrMagnitude();
	sqr_l[1] = (vertex_positions_[vertices.v0_] - vertex_positions_[vertices.v2_]).getSqrMagnitude();
	sqr_l[2] = (vertex_positions_[vertices.v0_] - vertex_positions_[vertices.v1_]).getSqrMagnitude();

	const int s_ix = FindSmallestIndex(sqr_l);
	if (sqr_l[s_ix] < sqr_min_edge_length) return s_ix;

	return -1;
}

int StaticTriangleOperations::getVertexIndex(const int& tri_ix, const int& v_ix) const
{
	const TV_INT &vertices(triangles_[tri_ix]);

	if (vertices.v0_ == v_ix) return 0;
	if (vertices.v1_ == v_ix) return 1;
	if (vertices.v2_ == v_ix) return 2;

	std::cout << "GetVertexIndex" << std::endl;
	exit(1);
}

void StaticTriangleOperations::calculateMeanCurvatureHelper(const int& tri_ix, const int& v0, T& area_mix_sum, TV& mean_curvature_sum) const
{
	static const T quater_pi = 3.141592f*0.25f;

	const TV_INT &vertices(triangles_[tri_ix]);

	const TV p0(vertex_positions_[vertices[v0]]);
	const TV p1(vertex_positions_[vertices[(v0 + 1) % 3]]);
	const TV p2(vertex_positions_[vertices[(v0 + 2) % 3]]);

	const T l1 = (p0 - p2).getMagnitudeDouble();
	const T l2 = (p0 - p1).getMagnitudeDouble();

	const T angle0(getAngle(p1 - p0, p2 - p0));
	const T angle1(getAngle(p2 - p1, p0 - p1));
	const T angle2(getAngle(p0 - p2, p1 - p2));

	const T area = crossProduct(p2 - p0, p1 - p0).getMagnitudeDouble();

	T area_mix;
	if (angle0 > quater_pi) area_mix = area*0.5f;
	else if (angle1 > quater_pi || angle2 > quater_pi) area_mix = area*0.25f;
	else area_mix = 0.125f*(l2*l2 / tan(angle2) + l1*l1 / tan(angle1));

	area_mix_sum += area_mix;

	mean_curvature_sum += (p0 - p1) / tan(angle2) + (p0 - p2) / tan(angle1);		//TODO: boundary case
}

T StaticTriangleOperations::getVoronoiArea(const int& tri_ix, const int& v0) const
{
	static const T quater_pi = 3.141592f*0.25f;

	const TV_INT &vertices(triangles_[tri_ix]);

	const TV p0(vertex_positions_[vertices[v0]]);
	const TV p1(vertex_positions_[vertices[(v0+1)%3]]);
	const TV p2(vertex_positions_[vertices[(v0+2)%3]]);

	const T l1 = (p0 - p2).getMagnitude();
	const T l2 = (p0 - p1).getMagnitude();

	const T angle0(getAngle(p1 - p0, p2 - p0));
	const T angle1(getAngle(p2 - p1, p0 - p1));
	const T angle2(getAngle(p0 - p2, p1 - p2));

	const T area = crossProduct(p2 - p0, p1 - p0).getMagnitude();

	if (angle0 > quater_pi) return area*0.5f;
	else if (angle1 > quater_pi || angle2 > quater_pi) return area*0.25f;
	else return 0.125f*(l2*l2 / tan(angle2) + l1*l1 / tan(angle1));
}

bool StaticTriangleOperations::checkFlip(const int& tri_ix, const int& v_chage, const TV& new_pos) const
{
	const TV_INT &vertices(triangles_[tri_ix]);

	TV p[3] = { vertex_positions_[vertices.v0_], vertex_positions_[vertices.v1_], vertex_positions_[vertices.v2_] };

	TV old_normal = crossProduct(p[2] - p[0], p[1] - p[0]);

	p[v_chage] = new_pos;

	TV new_normal = crossProduct(p[2] - p[0], p[1] - p[0]);

	if (dotProduct(old_normal, new_normal) <= (T)0) return true;
	else return false;
}

Box3D<T> StaticTriangleOperations::getAABB(const int& tri_ix) const
{
	const TV_INT &vertex_indices(triangles_[tri_ix]);

	const TV &v0(vertex_positions_[vertex_indices[0]]);
	const TV &v1(vertex_positions_[vertex_indices[1]]);
	const TV &v2(vertex_positions_[vertex_indices[2]]);

	return Box3D<T>(MIN3(v0.x_, v1.x_, v2.x_), MIN3(v0.y_, v1.y_, v2.y_), MIN3(v0.z_, v1.z_, v2.z_),
					 MAX3(v0.x_, v1.x_, v2.x_), MAX3(v0.y_, v1.y_, v2.y_), MAX3(v0.z_, v1.z_, v2.z_));
}

bool StaticTriangleOperations::isEdgeSharing(const int& tri0_ix, const int& tri1_ix) const
{
	const TV_INT &tri0(triangles_.values_[tri0_ix]), &tri1(triangles_.values_[tri1_ix]);

	for (int d0 = 0; d0 < 3; d0++)
	{
		const TV2_INT e0(tri0[(d0 + 1) % 3], tri0[(d0 + 2) % 3]);

		for (int d1 = 0; d1 < 3; d1++)
		{
			const TV2_INT e1(tri1[(d1 + 1) % 3], tri1[(d1 + 2) % 3]);

			if (e0.compareNonorderly(e1) == true) return true;
		}
	}

	return false;
}