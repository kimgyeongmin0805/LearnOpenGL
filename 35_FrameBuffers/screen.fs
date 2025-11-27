#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0, offset),
        vec2(offset, offset),
        vec2(-offset, 0),
        vec2(0, 0),
        vec2(offset, 0),
        vec2(-offset, -offset),
        vec2(0, -offset),
        vec2(offset, -offset)
    );

    float kernel[9] = float[](
         1,  2,  1,
         2,  4,  2,
         1,  2,  1
    );

    vec4 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = texture(screenTexture, TexCoords + offsets[i]);
    }

    vec4 color = vec4(0.0);
    for (int i = 0; i < 9; i++) {
        color += sampleTex[i] * kernel[i];
    }

    FragColor = color / 16;


}