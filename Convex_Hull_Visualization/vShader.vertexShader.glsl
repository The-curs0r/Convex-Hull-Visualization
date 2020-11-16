#version 450 core

in vec3 vertex;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out VS_OUT{
	float color;
}vs_out;

void main(void) {
	vs_out.color = vertex.z;
	gl_Position = proj_matrix * mv_matrix * vec4(vertex, 1.0f);
}