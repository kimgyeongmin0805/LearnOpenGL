#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float near;
uniform float far;
uniform float fogDensity;
uniform vec3 fogColor;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    float depthVec = exp(-pow(depth * fogDensity, 2.0));
    vec3 objColor = texture(texture1, TexCoords).rgb;
    vec3 result = mix(fogColor, objColor, depthVec);
    FragColor = vec4(result, 1.0);
}