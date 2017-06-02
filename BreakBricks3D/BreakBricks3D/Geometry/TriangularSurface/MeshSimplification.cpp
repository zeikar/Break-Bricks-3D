/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "MeshSimplification.h"

void MeshSimplification::update()
{
	Array1D<TV>  vertex_mean_curvatures_;

	surface_.calculateVertexMeanCurvatures(vertex_mean_curvatures_);

	triangles_flag_.init(surface_.triangles_.num_elements_, 0);		// -1: fixed (do not remove this time), -2: removed
	vertices_flag_.init(surface_.vertex_positions_.num_elements_, 0);	// -1: removed, -2: fixed

// 	for (int vix = 0; vix < vertices_flag_.num_elements_; vix++)
// 	{
// 		const T vertex_curvature = vertex_mean_curvatures_[vix].getMagnitudeDouble();
// 
// 		if (vertex_curvature >(double)kappa_) vertices_flag_[vix] = -2;
// 	}

	while (true)
	{
		surface_.findAdjacentTrianglesOfVertices();
		surface_.determineFaceAveragedVertexNormals();
		surface_.findEdgeTrianglesOfTriangles();

		if (removeShortEdges(min_edge_length_, kappa_) == 0) break;

		triangles_flag_.compactArray(triangles_flag_, -2);
		vertices_flag_.compactArray(vertices_flag_, -1);
	}

	surface_.findAdjacentTrianglesOfVertices();
	surface_.determineFaceAveragedVertexNormals();
	surface_.findEdgeTrianglesOfTriangles();

	std::cout << "tri = " << surface_.triangles_.num_elements_ << " vertices = " << surface_.vertex_positions_.num_elements_ << std::endl;
}

int MeshSimplification::removeShortEdges(const T& min_edge_length, const T kappa)
{
	const T sqr_length = POW2(min_edge_length);

	int num_removed_tri = 0;

	for (int tri_ix = 0; tri_ix < surface_.triangles_.num_elements_; tri_ix++)
	{
		if (triangles_flag_[tri_ix] < 0) continue;		// skip this triangle if this is already deleted or fixed.

		const int short_edge_ix = surface_.tri_ops_.getShortEdgeIndex(tri_ix, sqr_length);	// find an edge that is shorter than min_edge_length and shortest among three edges

		if (short_edge_ix == -1) continue;					// this triangle doesn't have shorter edge than sqr_length

		const int tri_ix1 = surface_.edge_tri_ix_of_triangles_[tri_ix][short_edge_ix];		// assert(t1 != -1)

		if (triangles_flag_[tri_ix1] < 0) continue;
		if (tri_ix1 < 0) continue;							// deleted elsewhere TODO: necessary?

		const TV_INT &v_ix(surface_.triangles_[tri_ix]);
		const int v_ix0(v_ix[(short_edge_ix + 1) % 3]), v_ix1(v_ix[(short_edge_ix + 2) % 3]);
		const int edge_ix1 = surface_.tri_ops_.getEdgeIndex(tri_ix1, v_ix0, v_ix1);			// short edge index of opposite triangle

		// skip if this edge flips induces edge redundancy
		// http://stackoverflow.com/questions/27049163/mesh-simplification-edge-collapse-conditions
		// 		if (tri_ops_.isEdgeSharing(edge_tri_ix_of_triangles_[tri_ix][(short_edge_ix + 1) % 3], edge_tri_ix_of_triangles_[tri_ix][(short_edge_ix + 2) % 3]) == true) continue;
		// 		if (tri_ops_.isEdgeSharing(edge_tri_ix_of_triangles_[tri_ix1][(edge_ix1 + 1) % 3], edge_tri_ix_of_triangles_[tri_ix1][(edge_ix1 + 2) % 3]) == true) continue;
		if (doesAdjTrisContact(v_ix0, v_ix1, surface_.triangles_[tri_ix1][edge_ix1], v_ix[short_edge_ix], tri_ix, tri_ix1) == true) continue;

		// skip if edge vertices have high curvatures TODO: use fixed vertex flag.
		// 		const T kappa_v0 = vertex_mean_curvatures_[v_ix0].getMagnitudeDouble();
		// 		const T kappa_v1 = vertex_mean_curvatures_[v_ix1].getMagnitudeDouble();
		// 		if (kappa_v0 >(double)kappa) continue;
		// 		if (kappa_v1 > (double)kappa) continue;

		// do not remove if vertex flag is set to don't-remove.
		if (vertices_flag_[v_ix0] == -2) continue;
		if (vertices_flag_[v_ix1] == -2) continue;

		// skip if one-neighbor triangles are fixed or removed
		if (containInvalidAdjTriangles(v_ix[0]) == true) continue;
		if (containInvalidAdjTriangles(v_ix[1]) == true) continue;
		if (containInvalidAdjTriangles(v_ix[2]) == true) continue;
		if (containInvalidAdjTriangles(surface_.triangles_[tri_ix1][edge_ix1]) == true) continue;

		const TV new_pos = (surface_.vertex_positions_[v_ix0] + surface_.vertex_positions_[v_ix1]) * (T)0.5;

		// check triangle flip http://graphics.stanford.edu/courses/cs468-10-fall/LectureSlides/08_Simplification.pdf page 47
		// skip if triangle flip is expected
		bool flip_expected = false;
		{const int adj_tri_start = surface_.start_ix_adj_tri_of_vertices_[v_ix0], adj_tri_end = surface_.start_ix_adj_tri_of_vertices_[v_ix0 + 1];
		for (int adj_tri_ix = adj_tri_start; adj_tri_ix < adj_tri_end; adj_tri_ix++)
		{
			const int v_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj_tri_ix];

			if (v_adj_tri_ix == -1) continue;
			if (v_adj_tri_ix == tri_ix) continue;
			if (v_adj_tri_ix == tri_ix1) continue;

			if (surface_.tri_ops_.checkFlip(v_adj_tri_ix, surface_.tri_ops_.getVertexIndex(v_adj_tri_ix, v_ix0), new_pos) == true)
			{
				flip_expected = true;
				break;
			}
		}}

		if (flip_expected == true) continue;

		{const int adj_tri_start = surface_.start_ix_adj_tri_of_vertices_[v_ix1], adj_tri_end = surface_.start_ix_adj_tri_of_vertices_[v_ix1 + 1];
		for (int adj_tri_ix = adj_tri_start; adj_tri_ix < adj_tri_end; adj_tri_ix++)
		{
			const int v_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj_tri_ix];

			if (v_adj_tri_ix == -1) continue;
			if (v_adj_tri_ix == tri_ix) continue;
			if (v_adj_tri_ix == tri_ix1) continue;

			if (surface_.tri_ops_.checkFlip(v_adj_tri_ix, surface_.tri_ops_.getVertexIndex(v_adj_tri_ix, v_ix1), new_pos) == true)
			{
				flip_expected = true;
				break;
			}
		}}

		if (flip_expected == true) continue;

		// tag the vertex sharing triangles of two triangles (to be deleted) so that they are not deleted this turn.
		tagAdjTriangles(v_ix[0], -1, 0);
		tagAdjTriangles(v_ix[1], -1, 0);
		tagAdjTriangles(v_ix[2], -1, 0);
		tagAdjTriangles(surface_.triangles_[tri_ix1][edge_ix1], -1, 0);

		removeEdge(tri_ix, tri_ix1, short_edge_ix, edge_ix1, v_ix0, v_ix1);

		num_removed_tri++;

		// 		// remove only one triangle at a time (test)
		// 		break;
	}

	std::cout << num_removed_tri << " triangles are removed " << std::endl;

	if (num_removed_tri == 0) return num_removed_tri;

	// 	if (CheckEdgeConnectivity()) std::cout << "Edge connectivity ok" << std::endl;
	// 	else std::cout << "Edge connectivity NOT ok " << std::endl;

	surface_.triangles_.compactArray(triangles_flag_, -2);
	surface_.vertex_positions_.compactArray(vertices_flag_, -1);

	// update old vertex indices to new vertex indices
	for (int tri = 0; tri < surface_.triangles_.num_elements_; tri++)
	{
		TV_INT &vertices(surface_.triangles_[tri]);

		vertices.v0_ = vertices_flag_[vertices.v0_];
		vertices.v1_ = vertices_flag_[vertices.v1_];
		vertices.v2_ = vertices_flag_[vertices.v2_];
	}

	return num_removed_tri;
}

bool MeshSimplification::doesAdjTrisContact(const int& v0_ix, const int& v1_ix, const int& v2_except, const int& v3_except, const int& tri0_ix_ignore, const int& tri1_ix_ignore) const
{
	const int adj0_tri_start = surface_.start_ix_adj_tri_of_vertices_[v0_ix], adj0_tri_end = surface_.start_ix_adj_tri_of_vertices_[v0_ix + 1];
	for (int adj0_tri_ix = adj0_tri_start; adj0_tri_ix < adj0_tri_end; adj0_tri_ix++)
	{
		const int v0_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj0_tri_ix];

		if (v0_adj_tri_ix == -1) continue;
		if (v0_adj_tri_ix == tri0_ix_ignore || v0_adj_tri_ix == tri1_ix_ignore) continue;

		const int adj1_tri_start = surface_.start_ix_adj_tri_of_vertices_[v1_ix], adj1_tri_end = surface_.start_ix_adj_tri_of_vertices_[v1_ix + 1];
		for (int adj1_tri_ix = adj1_tri_start; adj1_tri_ix < adj1_tri_end; adj1_tri_ix++)
		{
			const int v1_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj1_tri_ix];

			if (v1_adj_tri_ix == -1) continue;
			if (v1_adj_tri_ix == tri0_ix_ignore || v1_adj_tri_ix == tri1_ix_ignore) continue;

			if (surface_.tri_ops_.shareVertex(v0_adj_tri_ix, v1_adj_tri_ix, v2_except, v3_except) == true)
			{
				return true;
			}
		}
	}

	return false;
}

void MeshSimplification::tagAdjTriangles(const int& v_ix, const int& tri_flag, const int recursive_count)
{
	if (recursive_count > 1) return;

	const int adj_tri_start = surface_.start_ix_adj_tri_of_vertices_[v_ix], adj_tri_end = surface_.start_ix_adj_tri_of_vertices_[v_ix + 1];
	for (int adj_tri_ix = adj_tri_start; adj_tri_ix < adj_tri_end; adj_tri_ix++)
	{
		const int v_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj_tri_ix];

		if (v_adj_tri_ix == -1) continue;

		triangles_flag_[v_adj_tri_ix] = MIN2(triangles_flag_[v_adj_tri_ix], tri_flag);	// tag triangle not to remove in edge collapse
		//TODO: -2 is deleted triangle, overwriting tri_flag=-1 makes problem

		// two neighbor triangles
		// 		tagAdjTriangles(triangles_[v_adj_tri_ix].v0_, tri_flag, recursive_count + 1);
		// 		tagAdjTriangles(triangles_[v_adj_tri_ix].v1_, tri_flag, recursive_count + 1);
		// 		tagAdjTriangles(triangles_[v_adj_tri_ix].v2_, tri_flag, recursive_count + 1);
	}
}

void MeshSimplification::removeEdge(const int& tri_ix0, const int& tri_ix1, const int& edge_ix0, const int& edge_ix1, const int& v_ix0, const int& v_ix1)
{
	vertices_flag_[v_ix1] = -1;	// deleted

	// merge v0 and v1
	// remove v1 from triangles (replace v1 with v0)
	surface_.replaceVertexOfAdjTriangles(v_ix1, v_ix1, v_ix0);

	surface_.vertex_positions_[v_ix0] = (surface_.vertex_positions_[v_ix0] + surface_.vertex_positions_[v_ix1])*0.5f;

	// removing triangles
	removeEdgeSharingTriangles(tri_ix0, tri_ix1, edge_ix0, edge_ix1);
}

void MeshSimplification::removeEdgeSharingTriangles(const int &tri_ix0, const int & tri_ix1, const int& edge_ix0, const int& edge_ix1)
{
	// remove t0 and t1 from adj tri list of all vertices
	for (int d = 0; d < 3; d++) surface_.replaceAdjTriangle(surface_.triangles_[tri_ix0][d], tri_ix0, -1);
	for (int d = 0; d < 3; d++) surface_.replaceAdjTriangle(surface_.triangles_[tri_ix1][d], tri_ix1, -1);

	// fix adj tri of tri connectivity
	// 	ReplaceEdgeTriangle(edge_tri_ix_of_triangles_[tri_ix0][(edge_ix0 + 1) % 3], tri_ix0, edge_tri_ix_of_triangles_[tri_ix0][(edge_ix0 + 2) % 3]);
	// 	ReplaceEdgeTriangle(edge_tri_ix_of_triangles_[tri_ix0][(edge_ix0 + 2) % 3], tri_ix0, edge_tri_ix_of_triangles_[tri_ix1][(edge_ix0 + 1) % 3]);
	//   
	// 	ReplaceEdgeTriangle(edge_tri_ix_of_triangles_[tri_ix1][(edge_ix1 + 1) % 3], tri_ix1, edge_tri_ix_of_triangles_[tri_ix0][(edge_ix1 + 2) % 3]);
	// 	ReplaceEdgeTriangle(edge_tri_ix_of_triangles_[tri_ix1][(edge_ix1 + 2) % 3], tri_ix1, edge_tri_ix_of_triangles_[tri_ix1][(edge_ix1 + 1) % 3]);

	// remove triangles t0 t1
	surface_.emptifyTriangle(tri_ix0);
	surface_.emptifyTriangle(tri_ix1);

	triangles_flag_[tri_ix0] = -2;	// -2 means removed
	triangles_flag_[tri_ix1] = -2;
}

bool MeshSimplification::containInvalidAdjTriangles(const int& v_ix)
{
	const int adj_tri_start = surface_.start_ix_adj_tri_of_vertices_[v_ix], adj_tri_end = surface_.start_ix_adj_tri_of_vertices_[v_ix + 1];
	for (int adj_tri_ix = adj_tri_start; adj_tri_ix < adj_tri_end; adj_tri_ix++)
	{
		const int v_adj_tri_ix = surface_.adj_tri_ix_of_vertices_[adj_tri_ix];

		if (v_adj_tri_ix == -1) continue;

		if (triangles_flag_[v_adj_tri_ix] < 0) return true;	// if this triangle is fixed or deleted elsewhere.
	}

	return false;
}
