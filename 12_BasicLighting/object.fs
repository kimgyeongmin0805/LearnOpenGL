#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	vec3 lightVector = normalize(lightPos - FragPos);
	vec3 viewVector = normalize(viewPos - FragPos);
	vec3 reflectVector = reflect(-lightVector, Normal);

	float ambientStrength = 0.1;
	vec3 ambientColor = ambientStrength * lightColor;

	float diffuse = max(dot(Normal, lightVector), 0.0);
	vec3 diffuseColor = diffuse * lightColor;

	float specularStrength = 0.5;
	float specular = pow(max(dot(viewVector, reflectVector), 0.0), 32);
	vec3 specularColor = specularStrength * specular * lightColor;

	vec3 result = (ambientColor + diffuseColor + specularColor) * objectColor;
	FragColor = vec4(result, 1.0);
}