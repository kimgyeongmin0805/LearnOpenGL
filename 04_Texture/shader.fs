#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main() {
	FragColor = mix(texture(ourTexture, ourTexCoord), texture(ourTexture2, ourTexCoord), 0.2) * vec4(ourColor, 1.0);
}