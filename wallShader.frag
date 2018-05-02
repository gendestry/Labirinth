#version 330 core
out vec4 color;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

uniform vec3 viewPos;
uniform vec3 fragPos;

uniform sampler2D tex;

void main() {
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(vec3(0.5,1.0,0.2)); 
    float diffk = max(dot(norm, lightDir), 0.15);

	vec3 viewDir = normalize(fragPos - viewPos);
	float fresnel = 1;// - min(pow(dot(normal, viewDir), 1), 0.9);

	color =  fresnel * diffk * texture(tex, texcoord);
}