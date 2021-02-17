#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;
uniform bool isPerspective;

float linearizeDepth(float depth){
	float z = 2 * depth - 1;
	return 2 * nearPlane * farPlane / farPlane + nearPlane - z * (farPlane - nearPlane);
}

void main() {
	float depthValue = texture(depthMap, TexCoords).r;
	if(isPerspective) {
		FragColor = vec4(vec3(linearizeDepth(depthValue) / farPlane), 1.0);
	} else {
		FragColor = vec4(vec3(depthValue), 1.0);
	}
}