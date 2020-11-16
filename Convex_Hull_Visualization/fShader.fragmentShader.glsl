#version 450 core

out vec4 color;

in VS_OUT{
	float color;
}fs_in;

void main(void) {
	color = vec4(vec3(fs_in.color), 1.0f);
}