#version 440 core
out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
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

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // camera position

uniform Material material;
uniform Light light;

void main () {

	vec3 ligthDir = normalize(light.position - fragPos);// light dir
	
	float ambientStrength = 0.1;
	//vec3 ambientColor = ambientStrength * lightColor;
	//vec3 resultAmbient = ambientColor * objectColor;

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, ligthDir), 0.0); // diffuse impact
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // diffuse color
	//vec3 resultDiffuse = (ambientColor + diffuse)* objectColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPos - fragPos); // view direction
	vec3 refelection = reflect(-ligthDir, norm);
	float spec = pow(max(dot(viewDirection, refelection), 0.0), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));
	//vec3 resultSpecular = (ambientColor + diffuse + specular) * objectColor;

	//..::softning::../
	float theta = dot(ligthDir, normalize(-light.direction));
	float epsilion = light.cuttOff - light.outerCutOff; // different inner cone cutoff and outer cone cutoff
	float intensity = clamp(((theta - light.outerCutOff) / epsilion), 0.0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	//..::point light distance::..//
	float distance = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);	
//	if(theta > light.cuttOff) {
//	} else {
//		fragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
//	}
}