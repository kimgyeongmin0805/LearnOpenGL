#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texture_cube;

void main() {
    FragColor = texture(texture_cube, TexCoords);
}