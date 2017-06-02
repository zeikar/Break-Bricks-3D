/*
Authored by Prof. Jeong-Mo Hong, CSE Dongguk University
for Introduction to Computer Graphics, 2017 Spring
*/
#version 330

//uniform mat4 gl_ModelViewMatrix;
//uniform mat4 gl_ProjectionMatrix;
uniform mat4 mvp;

in vec3 a_pos;
in vec3 a_nor;

out vec4 v;
out vec4 N;

void main()
{
	//v = gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_pos, 1.0);
	//N = normalize(gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_nor, 0.0f));
	v = mvp * vec4(a_pos, 1.0);
	N = normalize(mvp * vec4(a_nor, 0.0f));

	gl_Position = v;
};