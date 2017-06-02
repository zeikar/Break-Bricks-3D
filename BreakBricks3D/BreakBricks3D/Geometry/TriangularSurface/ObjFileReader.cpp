/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#include "ObjFileReader.h"

#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

void ObjFileReader::ReadOBJ(const char *filename)
{
	using namespace std;

	bool first_vertex = true;		// to determine AABB

	clock_t start = clock();

	if (use_cout) std::cout << "Start reading OBJ file " << filename << std::endl;

	// to check if this obj file contains vt or vn data 
	bool read_vt(false), read_vn(false);

	ifstream file(filename);

	// check if file is correctly opened
	if (file.is_open() == false){ std::cout << filename << " does not exist. Program terminated." << std::endl; exit(-1); }

	char c[255];

	while (true)
	{
		file >> c;

		if (file.eof() != 0) break;						// finish reading if file is ended

		if (strcmp(c, "#") == 0) file.getline(c, 255);  // comments (less than 255 characters)
		else if (strcmp(c, "v") == 0) // vertices
		{
			float x, y, z;
			file >> x >> y >> z;

			pos_stack_.pushBack() = Vector3D<float>(x, y, z);

			if (first_vertex)
			{
				x_min_ = x_max_ = x;
				y_min_ = y_max_ = y;
				z_min_ = z_max_ = z;

				first_vertex = false;
			}
			else
			{
				x_min_ = min(x_min_, x);
				y_min_ = min(y_min_, y);
				z_min_ = min(z_min_, z);

				x_max_ = max(x_max_, x);
				y_max_ = max(y_max_, y);
				z_max_ = max(z_max_, z);
			}

			if (use_cout) std::cout << x << " " << y << " " << z << std::endl;
		}
		else if (strcmp(c, "vt") == 0)
		{
			read_vt = true; 

			float u, v;
			file >> u >> v;

			uv_stack_.pushBack() = TV2(u, v);
		
		} 
		else if (strcmp(c, "vn") == 0) 
		{
			read_vn = true;

			float nx, ny, nz;
			file >> nx >> nz >> ny;

			normal_stack_.pushBack() = TV3(nx, ny, nz);
		}
		else if (strcmp(c, "f") == 0)
		{
			int v[3], vt[3], vn[3];
			if (read_vt == true && read_vn == true)
			{
				for (int i = 0; i < 3; i++)
				{
					file >> v[i]; file.get(c, 2);
					file >> vt[i]; file.get(c, 2);
					file >> vn[i];

					v[i]--;
					vt[i]--;
					vn[i]--;
				}
			}
			else if (read_vt == false && read_vn == true)
			{
				for (int i = 0; i < 3; i++)
				{
					file >> v[i]; file.get(c, 2); file.get(c, 2);
					file >> vn[i];
					v[i]--;
					vn[i]--;
				}
			}
			else if (read_vt == true && read_vn == false)
			{
				for (int i = 0; i < 3; i++)
				{
					file >> v[i]; file.get(c, 2); file.get(c, 2);
					file >> vt[i];
					v[i]--;
					vt[i]--;
				}
			}
			else if (read_vt == false && read_vn == false)
			{
				for (int i = 0; i < 3; i++)
				{
					file >> v[i];
					v[i]--;
				}
			}

			ix_stack_.pushBack() = TV_INT(v[0], v[1], v[2]);
//			ix_stack_.pushBack() = TV_INT(v[2], v[1], v[0]);

			if (read_vt == true) {
				uv_ix_stack_.pushBack() = TV_INT(vt[0], vt[1], vt[2]);
			}

			if (read_vn == true) {
				nor_ix_stack_.pushBack() = TV_INT(vn[0], vn[1], vn[2]);
			}

			if (use_cout) std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
		}
	}
	file.clear();
	file.close();

	if (use_cout) std::cout << "Reading complete." << std::endl;

	clock_t finish = clock();
	std::cout << (double)(finish - start) / (double)CLOCKS_PER_SEC << std::endl;
	std::cout << "# of vertices = " << pos_stack_.num_elements_ << " # of triangles = " << ix_stack_.num_elements_ << std::endl;

	start = clock();

	//
	pos_stack_.copyToArray(posArray);
	normal_stack_.copyToArray(norArray);
	uv_stack_.copyToArray(texArray);

	ix_stack_.copyToArray(posIdxArray);
	nor_ix_stack_.copyToArray(norIdxArray);
	uv_ix_stack_.copyToArray(texIdxArray);
}

void ObjFileReader::GetPositionsFromTri(const int n, TV3 pos[3]) {	
	TV_INT& idx = posIdxArray.values_[n];
	pos[0] = posArray.values_[idx.x_];
	pos[1] = posArray.values_[idx.y_];
	pos[2] = posArray.values_[idx.z_];
}

void ObjFileReader::GetTexCoordsFromTri(const int n, TV2 tex[3]) {
	TV_INT& idx = texIdxArray.values_[n];
	tex[0] = texArray.values_[idx.x_];
	tex[1] = texArray.values_[idx.y_];
	tex[2] = texArray.values_[idx.z_];
}

TV3 ObjFileReader::GetNormalFromTri(const int n) {
	TV_INT& idx = posIdxArray.values_[n];
	TV3& p0 = posArray.values_[idx.x_];
	TV3& p1 = posArray.values_[idx.y_];
	TV3& p2 = posArray.values_[idx.z_];

	TV3 nor = crossProduct(p1 - p0, p2 - p0);
	return nor.getNormalized();
}

TV3 ObjFileReader::GetCenter() {
	TV3 center(0.0, 0.0, 0.0);
	for (int i = 0; i < posArray.num_elements_; i++) {
		center += posArray.values_[i];
	}
	return center / (float)posArray.num_elements_;
}

const glm::vec3 ObjFileReader::GetCenterAABB() const 
{
	const glm::vec3 center((x_min_ + x_max_)*0.5f, (y_min_ + y_max_)*0.5f, (z_min_ + z_max_)*0.5f);

	return center;
}

const float ObjFileReader::GetScaleAABB() const
{
	using namespace std;

	float scale = max(max(x_max_ - x_min_, y_max_ - y_min_), z_max_ - z_min_);
	if (scale != 0.0f) scale = 1.0f / scale;

	return scale;
}

const glm::vec3 ObjFileReader::GetScaleVecAABB() const
{
	using namespace std;

	float scale = max(max(x_max_ - x_min_, y_max_ - y_min_), z_max_ - z_min_);
	if (scale != 0.0f) scale = 1.0f / scale;

	return glm::vec3(scale, scale, scale);
}


void ObjFileReader::GetTriangleArray(Array1D<TV3_INT>& tri_arr)
{
	ix_stack_.copyToArray(tri_arr);
}

void ObjFileReader::GetIndexArray(Array1D<unsigned short>& idx_arr) {

	Array1D<TV3_INT> tempArray;
	ix_stack_.copyToArray(tempArray);
	idx_arr.init(tempArray.num_elements_ * 3);

	for (int i = 0; i < tempArray.num_elements_; i++) {

		idx_arr.values_[i * 3 + 0] = tempArray.values_[i].x_;
		idx_arr.values_[i * 3 + 1] = tempArray.values_[i].y_;
		idx_arr.values_[i * 3 + 2] = tempArray.values_[i].z_;
	}
}

void ObjFileReader::GetPositionArray(Array1D<float>& pos_arr) {

	Array1D<TV3> tempArray;
	pos_stack_.copyToArray(tempArray);
	pos_arr.init(pos_stack_.num_elements_ * 3);

	for (int i = 0; i < tempArray.num_elements_; i++) {

		pos_arr.values_[i * 3 + 0] = tempArray.values_[i].x_;
		pos_arr.values_[i * 3 + 1] = tempArray.values_[i].y_;
		pos_arr.values_[i * 3 + 2] = tempArray.values_[i].z_;
	}
}

void ObjFileReader::GetNormalArray(Array1D<float>& nor_arr) {

	Array1D<TV3> tempArray;
	normal_stack_.copyToArray(tempArray);
	nor_arr.init(normal_stack_.num_elements_ * 3);

	for (int i = 0; i < tempArray.num_elements_; i++) {

		nor_arr.values_[i * 3 + 0] = tempArray.values_[i].x_;
		nor_arr.values_[i * 3 + 1] = tempArray.values_[i].y_;
		nor_arr.values_[i * 3 + 2] = tempArray.values_[i].z_;
	}
}

void ObjFileReader::GetPositionArray(Array1D<TV3>& new_pos_arr) {

	pos_stack_.copyToArray(new_pos_arr);

}

void ObjFileReader::GetUVCoordArray(Array1D<TV2>& uv_arr, Array1D<TV3_INT>& tri_arr) {

	uv_stack_.copyToArray(uv_arr);
	uv_ix_stack_.copyToArray(tri_arr);
}

void ObjFileReader::GetNormalArray(Array1D<TV3>& new_normal_arr) {

	Array1D<TV3> normal_arr;
	normal_stack_.copyToArray(normal_arr);

	new_normal_arr.init(normal_arr.num_elements_);

	Array1D<TV_INT> ix_arr;
	Array1D<TV_INT> uv_ix_arr;

	ix_stack_.copyToArray(ix_arr);
	uv_ix_stack_.copyToArray(uv_ix_arr);


	for (int x = 0; x < ix_arr.num_elements_; x++) {

		new_normal_arr[ix_arr[x].i_] = normal_arr[uv_ix_arr[x].i_];
		new_normal_arr[ix_arr[x].j_] = normal_arr[uv_ix_arr[x].j_];
		new_normal_arr[ix_arr[x].k_] = normal_arr[uv_ix_arr[x].k_];
	}
}

