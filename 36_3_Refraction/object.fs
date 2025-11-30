#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 cameraPos;
uniform samplerCube texture_cube;

void main() {
    float ratio = 1.0 / 1.52;
    vec3 camera_direction = normalize(FragPos - cameraPos);
    vec3 normal = normalize(Normal);
    vec3 refract_direction = refract(camera_direction, normal, ratio);

    FragColor = texture(texture_cube, refract_direction);
}