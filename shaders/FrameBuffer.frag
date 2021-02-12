#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    //...gray scale implementation
    //vec3 col = vec4(vec3(1 - ));
    //texture(screenTexture, TexCoords).rgb;
    //FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b; //more accurate gray scale
    //FragColor = vec4(average, average, average, 1.0);

    //..Kernal Effects
    const float offset = 1.0f / 300.0f;
    vec2 offsets[9] = vec2[](
        vec2(-offset,offset), vec2(0.0f, offset), vec2(offset, offset),
        vec2(-offset,offset), vec2(0.0f, offset), vec2(offset, offset),
        vec2(-offset,-offset), vec2(0.0f, -offset), vec2(offset, -offset)
    );

    float kernel[9] = float[] (
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

//    float kernel[9] = float[](
//        1.0 / 32, 2.0 / 32, 1.0 / 32,
//        2.0 / 32, 4.0 / 32, 2.0 / 32,
//        1.0 / 32, 2.0 / 32, 1.0 / 32  
//    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * kernel[i];
    }
    FragColor = vec4(col, 1.0);
} 