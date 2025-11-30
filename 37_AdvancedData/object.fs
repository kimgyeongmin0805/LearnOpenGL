#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform samplerCube texture_cube;
uniform vec3 cameraPos;

void main() {
    vec3 camera_direction = normalize(FragPos - cameraPos);
    vec3 normal = normalize(Normal);
    vec3 reflect_direction = reflect(camera_direction, normal);

    FragColor = texture(texture_cube, reflect_direction);
}