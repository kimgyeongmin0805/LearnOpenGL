#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// normal에는 translate 성분이 들어가면 안된다.
	// transpose와 inverse는 CPU에서 실행하는 것이 효율적
    Normal = normalize(mat3(model) * aNormal);  
	FragPos = vec3(model * vec4(aPos, 1.0));
}