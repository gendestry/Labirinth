#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 position;
out vec3 normal;

void main() {
	position = pos;
	normal = norm;
	gl_Position = proj * view * model * vec4(pos, 1.0);
}