#version 440 core
out vec4 fragColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	fragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
	//FragColor = texture(texture1, TexCoord);
}