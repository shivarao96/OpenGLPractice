#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

in VS_OUT{
	vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool showBlinLighting;
uniform bool showAdvanceLighting;

void main()
{    
	if(showAdvanceLighting) {
		vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
		// ambient
		vec3 ambient = 0.5 * color;
		// diffuse
		vec3 normal = normalize(fs_in.Normal);
		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * color;
		// specular
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = 0.0;
		if(showBlinLighting) {
			vec3 halfWayDir = normalize(lightDir + viewDir);
			spec = pow(max(dot(normal, halfWayDir), 0.0), 32.0);
		} else {
			spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
		}
		vec3 specular = vec3 (1.0) * spec;
		FragColor = vec4(ambient + diffuse + specular, 1.0);
	} else {
		FragColor = texture(texture_diffuse1, fs_in.TexCoords);
	}
}