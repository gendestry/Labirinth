#version 330 core
out vec4 color;

in vec3 position;
in vec3 normal;

void main() {
	vec3 lightPos = vec3(1000.0, 1000.0, 500.0);
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diffk = max(dot(norm, lightDir), 0.1);

	color =  diffk * vec4(1.0);
}