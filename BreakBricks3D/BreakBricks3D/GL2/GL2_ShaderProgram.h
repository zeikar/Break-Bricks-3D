/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class GL2_ShaderProgram
{
public:
	GLuint program_id_;

	~GL2_ShaderProgram()
	{
		glDeleteProgram(program_id_);
	}

	void readShaders(const char * vertex_file_path, const char * fragment_file_path)
	{
		program_id_ = readShadersFromFiles(vertex_file_path, fragment_file_path);
	}

	void sendUniform(const glm::mat4 matrix4, const std::string& name) const
	{
		GLint loc = glGetUniformLocation(program_id_, name.c_str());
		if (loc != -1)
		{
			glUniformMatrix4fv(loc, 1, false, &matrix4[0][0]);
		}
	}

	void sendUniform(const glm::vec4 v4, const std::string& name) const
	{
		GLint loc = glGetUniformLocation(program_id_, name.c_str());
		if (loc != -1)
		{
			glUniform4fv(loc, 1, &v4[0]);
		}
	}

	void sendUniform(const float& f, const std::string& name) const
	{
		GLint loc = glGetUniformLocation(program_id_, name.c_str());
		if (loc != -1)
		{
			glUniform1f(loc,f);
		}
	}

private:
	GLuint readShadersFromFiles(const char * vertex_file_path, const char * fragment_file_path);


};

