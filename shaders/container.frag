#version 440 core
out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // camera position

uniform Material material;
uniform Light light;

void main () {
	float ambientStrength = 0.1;
	//vec3 ambientColor = ambientStrength * lightColor;
	//vec3 resultAmbient = ambientColor * objectColor;

	vec3 ambient = material.ambient * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 ligthDir = normalize(light.position - fragPos);// light dir
	float diff = max(dot(norm, ligthDir), 0.0); // diffuse impact
	vec3 diffuse = vec3(1.0f) * (material.diffuse * diff) * light.diffuse; // diffuse color
	//vec3 resultDiffuse = (ambientColor + diffuse)* objectColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPos - fragPos); // view direction
	vec3 refelection = reflect(-ligthDir, norm);
	float spec = pow(max(dot(viewDirection, refelection), 0.0), material.shininess);
	vec3 specular = vec3(1.0f) * (material.specular * spec) * light.specular;
	//vec3 resultSpecular = (ambientColor + diffuse + specular) * objectColor;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}