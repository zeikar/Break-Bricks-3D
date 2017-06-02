#version 330

uniform mat4 gl_ModelViewMatrix;
uniform mat4 gl_ProjectionMatrix;

in vec3 a_pos;
in vec3 a_nor;

out vec4 v;
out vec4 N;

void main()
{
	v = gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_pos, 1.0);
	N = normalize(gl_ModelViewMatrix * gl_ProjectionMatrix * vec4(a_nor, 0.0f));

	gl_Position = v;	
};