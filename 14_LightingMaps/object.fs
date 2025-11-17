#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float time;

void main() {
	// texture
	vec3 pigment = texture(material.diffuse, TexCoord).rgb;
	vec3 pigment_specular = texture(material.specular, TexCoord).rgb;
	
	// ambient
	vec3 ambientColor = pigment * light.ambient;
	// diffuse
	vec3 lightVector = normalize(light.position - FragPos);
	float diffuse = max(dot(Normal, lightVector), 0.0);
	vec3 diffuseColor = (pigment * diffuse) * light.diffuse;
	// view
	vec3 viewVector = normalize(viewPos - FragPos);
	vec3 reflectVector = reflect(-lightVector, Normal);
	float specular = pow(max(dot(viewVector, reflectVector), 0.0), material.shininess);
	vec3 specularColor = (pigment_specular * specular) * light.specular;
	// emission
	vec3 emissionColor = texture(material.emission, TexCoord + vec2(0.0, time)).rgb * pow(sin(time * 5), 2);
	
	// result
	vec3 result = ambientColor + diffuseColor + specularColor;
	
	if (pigment_specular == vec3(0.0)) {
		result += emissionColor;
	}

	FragColor = vec4(result, 1.0);
}