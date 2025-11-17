#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {

	vec3 ambientColor = material.ambient * light.ambient;
	
	vec3 lightVector = normalize(light.position - FragPos);
	float diffuse = max(dot(Normal, lightVector), 0.0);
	vec3 diffuseColor = (material.diffuse * diffuse) * light.diffuse;

	vec3 viewVector = normalize(viewPos - FragPos);
	vec3 reflectVector = reflect(-lightVector, Normal);
	float specular = pow(max(dot(viewVector, reflectVector), 0.0), material.shininess);
	vec3 specularColor = (material.specular * specular) * light.specular;

	vec3 result = ambientColor + diffuseColor + specularColor;
	FragColor = vec4(result, 1.0);
}