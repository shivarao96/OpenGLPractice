#version 440 core
out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skyBoxTex;

void main() {
	FragColor = texture(skyBoxTex, TexCoords);
}