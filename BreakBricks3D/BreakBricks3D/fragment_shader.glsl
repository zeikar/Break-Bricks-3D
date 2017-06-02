#version 330

in vec4 v;
in vec4 N;

out vec4 frag_colour;

void main()
{
	// Use 'uniform' to send light/material setting
	// Hardcoded here for lecturing purposes
	vec4 light_pos = vec4(0.0f, 0.3f, 0.0f, 1.0f);
	vec4 lm_amb = vec4(0.1f, 0.01f, 0.01f, 1.0f);
	vec4 lm_dif = vec4(1.0f, 0.1f, 0.1f, 1.0f);
	vec4 lm_spe = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	float mat_shininess = 5;

	vec3 L = (normalize(light_pos.xyz - v.xyz)).xyz;
	vec3 E = (normalize(-v.xyz)).xyz;		// we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = (normalize(-reflect(L.xyz, N.xyz))).xyz;

	//calculate Ambient Term:  
	vec4 Iamb = lm_amb;

	//calculate Diffuse Term:  
	vec4 Idiff = lm_dif * max(dot(N.xyz, L.xyz), 0.0);

	// calculate Specular Term:
	vec4 Ispec = lm_spe
		* pow(max(dot(R, E), 0.0), 0.3*mat_shininess);

	// write Total Color:  
	gl_FragColor = Iamb + Idiff + Ispec;
};