#version 440 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float shadowCalculation(vec4 fragPosLightSpace, float bias){
	
	vec3 projCoords    = fragPosLightSpace.xyz / fragPosLightSpace.w; // convert light space frag psoition in clip space to NDC
	projCoords         = projCoords * 0.5 + 0.5; //clamping the value in between [0,1]
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow;
	if (currentDepth > 1.0){
		shadow         = 0.0;
	} else {
		//shadow       = currentDepth - bias > closestDepth ? 1.0 : 0.0;
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x){
			for(int y = -1; y <= 1; ++y) {
				float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			}	
		}
		shadow /= 9.0;
	}
	return shadow;
}

void main() {
	vec3 color            = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 normal           = normalize(fs_in.Normal);
	vec3 lightColor       = vec3(1.0);
	// ambient
	vec3 ambient          = 0.3 * color;
	// diffuse
	vec3 lightDir         = normalize(lightPos - fs_in.FragPos);
	float diff            = max(dot(lightDir, normal), 0.0);
	vec3 diffuse          = diff * lightColor;
	// specular
	vec3 viewDir          = normalize(viewPos - fs_in.FragPos);
	float spec            = 0.0;
	vec3 halfWayDirection = normalize(lightDir + viewDir);
	spec                  = pow(max(dot(normal, halfWayDirection), 0.0), 128.0);
	vec3 specular         = spec * lightColor;
	// calculate shadow
	float bias            = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow          = shadowCalculation(fs_in.FragPosLightSpace, bias);
	vec3 lighting         = (ambient + (1.0 - shadow) * (diffuse)) * color;
	
	FragColor             = vec4(lighting, 1.0);
}