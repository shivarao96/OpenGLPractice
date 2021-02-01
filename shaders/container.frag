#version 440 core
out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // camera position

void main () {
	float ambientStrength = 0.1;
	vec3 ambientColor = ambientStrength * lightColor;
	vec3 resultAmbient = ambientColor * objectColor;

	vec3 norm = normalize(Normal);
	vec3 ligthDir = normalize(lightPos - fragPos);// light dir
	float diff = max(dot(norm, ligthDir), 0.0); // diffuse impact
	vec3 diffuse = diff * lightColor; // diffuse color
	vec3 resultDiffuse = (ambientColor + diffuse)* objectColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPos - fragPos); // view direction
	vec3 refelection = reflect(-ligthDir, norm);
	float spec = pow(max(dot(viewDirection, refelection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	vec3 resultSpecular = (ambientColor + diffuse + specular) * objectColor;

	fragColor = vec4(resultSpecular, 1.0);
}