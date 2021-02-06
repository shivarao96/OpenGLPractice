#version 440 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cuttOff;
	float outerCutOff;
};

struct DirectionLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionLight dirLight;

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight pointLights;

struct SpotLight {
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cuttOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
};

uniform SpotLight sptLight;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // camera position

uniform Material material;
uniform Light light;

//------
vec3 calDirectionLight(DirectionLight dirLight, vec3 normal, vec3 viewDir){
	
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 calPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir) {
	vec3 lightDir  = normalize(light.position - fragPosition);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	float distanceF = length(lightPos - fragPosition);
	float attenuation  = 1 / (light.constant + light.linear * distanceF + light.quadratic * (distanceF * distanceF));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	float theta = max(dot(lightDir, -light.direction), 0.0);
	float epsilon = light.cuttOff - light.outerCutOff;
	float intensity = clamp(((theta - light.outerCutOff) / epsilon), 0.0, 1.0);

	float distanceF = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear * distanceF + light.quadratic * (distanceF * distanceF));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main () {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = calDirectionLight(dirLight, norm, viewDir);
	result += calPointLight(pointLights, norm, FragPos, viewDir);
	result += calSpotLight(sptLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0);
}