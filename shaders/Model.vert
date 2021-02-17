#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT{
	vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} vs_in;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_in.TexCoords = aTexCoords;
    vs_in.Normal = aNormal;
    vs_in.FragPos = aPos;
    TexCoords = aTexCoords;       
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}