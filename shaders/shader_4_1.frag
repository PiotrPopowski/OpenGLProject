#version 430 core
uniform vec3 objectColor;
in vec3 interpNormal;
uniform vec3 light;
void main()
{
	vec3 v3 = normalize(light);
	float x = dot(interpNormal,v3);
	gl_FragColor = vec4(objectColor*x, 1.0);
}