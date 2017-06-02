/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <GL/glew.h> // include glew first
#include "../Geometry/TriangularSurface/StaticTriangularSurface.h"
#include <glm\glm.hpp>
#include "GL2_ShaderProgram.h"
#include "GL2_Light.h"
#include "GL2_Material.h"
#include <glm/gtx/component_wise.hpp>

class GL2_Object
{
public:
	Array1D<GLuint> vbo_;
	int num_elements_ = 0;

	glm::vec4 light_position_ = glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);
	glm::vec4 light_product_ambient_ = glm::vec4(0.1f, 0.01f, 0.01f, 1.0f);
	glm::vec4 light_product_diffuse_ = glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
	glm::vec4 light_product_specular_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	float mat_shininess_ = 5.0;

	GL2_Material mat_;

	GL2_Object()
	{
	}

	~GL2_Object()
	{
		glDeleteBuffersARB(vbo_.num_elements_, vbo_.values_);
	}

	void initPhongSurface(const StaticTriangularSurface& surface)
	{
		vbo_.init(3); // vertex, normal, connectivity
		glGenBuffers(vbo_.num_elements_, vbo_.values_);

		updatePhongSurface(surface);
	}

	void updatePhongSurface(const StaticTriangularSurface& surface)
	{
		num_elements_ = surface.triangles_.num_elements_ * 3;

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, surface.vertex_positions_.num_elements_ * sizeof(float) * 3,
			surface.vertex_positions_.values_, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ARRAY_BUFFER, surface.vertex_normals_.num_elements_ * sizeof(float) * 3,
			surface.vertex_normals_.values_, GL_STATIC_DRAW);

		Array1D<Vector3D<unsigned int> > tri_ix;
		surface.getUnsignedIntTriangles(tri_ix);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_ix.num_elements_ * sizeof(unsigned int) * 3,
			surface.triangles_.values_, GL_STATIC_DRAW);
	}

	void drawWithShader(const GL2_ShaderProgram& program)
	{
		//glUseProgram(shader_programme);	// activate your shader!

		//TODO: use one 'lightproduct' uniform for amb, dif, and spe.
		program.sendUniform(light_position_, "light_position");
		program.sendUniform(light_product_ambient_, "light_product_ambient");
		program.sendUniform(light_product_diffuse_, "light_product_diffuse");
		program.sendUniform(light_product_specular_, "light_product_specular");
		program.sendUniform(mat_shininess_, "mat_shininess");

		// draw here
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer
		(
			0,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer
		(
			1,                  // attribute 0
			3,                  // size (r, g, b)
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glLineWidth(1);
		glPolygonMode(GL_FRONT, GL_LINES);
		glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
	}

	void drawPhongSurface()
	{
		glUseProgram(0);

		glEnable(GL_LIGHTING);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[2]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);	

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	void applyLighting(const GL2_Light& light)
	{
		light_product_ambient_ = light.ambient_ * mat_.ambient_ + mat_.emission_;
		light_product_diffuse_ = light.diffuse_ * mat_.diffuse_;
		light_product_specular_ = light.specular_ * mat_.specular_;

		light_position_ = light.position_;
		mat_shininess_ = mat_.shininess_;
	}
};
