#pragma once

#include "DataStructure/Vector2D.h"
#include "DataStructure/Vector3D.h"

typedef float T;
typedef double D;

typedef Vector2D<T> TV2;
typedef Vector2D<D> DV2;
typedef Vector2D<int> TV2_INT;

typedef Vector3D<T> TV3;
typedef Vector3D<D> DV3;
typedef Vector3D<int> TV3_INT;

typedef Vector3D<T> TV;
typedef Vector3D<D> DV;
typedef Vector3D<int> TV_INT;

#define TRAVERSE_VERTICES	std::vector <DynamicVertex*>::iterator itr_vertex;		for(itr_vertex = vertices_.begin(); itr_vertex != vertices_.end(); itr_vertex ++)
#define TRAVERSE_EDGES		std::list <DynamicEdge*>::iterator itr_edge;			for(itr_edge = edges_.begin(); itr_edge != edges_.end(); itr_edge ++)
#define TRAVERSE_TRIANGLES 	std::list <DynamicTriangle*>::iterator itr_triangle;	for(itr_triangle = triangles_.begin(); itr_triangle != triangles_.end(); itr_triangle ++)