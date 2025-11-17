#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 viewPos;
uniform float time;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 calDirLight(DirLight light, vec3 viewVector, vec3 normal);
vec3 calPointLight(PointLight light, vec3 viewVector, vec3 normal, vec3 FragPos);
vec3 calSpotLight(SpotLight light, vec3 viewVector, vec3 normal, vec3 FragPos);

void main() {
	vec3 result = vec3(0.0, 0.0, 0.0);

	vec3 viewVector = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);
	
	// DIRECTIONAL LIGHT
	result += calDirLight(dirLight, viewVector, normal);

	// POINT LIGHT
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += calPointLight(pointLights[i], viewVector, normal, FragPos);
	}

	// SPOT LIGHT
	result += calSpotLight(spotLight, viewVector, normal, FragPos);

	// EMISSION
	vec3 emission = texture(material.emission, TexCoord + vec2(0.0, time)).rgb * pow(sin(time * 5), 2);

	if (texture(material.specular, TexCoord).rgb != vec3(0.0, 0.0, 0.0)) {
		emission *= 0;
	}

	result += emission;

	FragColor = vec4(result, 1.0);
}

vec3 calDirLight(DirLight light, vec3 viewVector, vec3 normal) {
	vec3 lightVector = normalize(-light.direction);
	
	vec3 ambient = texture(material.diffuse, TexCoord).rgb * light.ambient;
	
	float diff = max(dot(viewVector, lightVector), 0.0);
	vec3 diffuse = texture(material.diffuse, TexCoord).rgb * light.diffuse * diff;

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(viewVector, reflectVector), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).rgb * light.specular * spec;

	return (ambient + diffuse + specular);
}

vec3 calPointLight(PointLight light, vec3 viewVector, vec3 normal, vec3 FragPos) {
	vec3 lightVector = normalize(light.position - FragPos);
	
	vec3 ambient = texture(material.diffuse, TexCoord).rgb * light.ambient;

	float diff = max(dot(lightVector, normal), 0.0);
	vec3 diffuse = texture(material.diffuse, TexCoord).rgb * light.diffuse * diff;

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(reflectVector, viewVector), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).rgb * light.specular * spec;

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calSpotLight(SpotLight light, vec3 viewVector, vec3 normal, vec3 FragPos) {
	vec3 lightVector = normalize(light.position - FragPos);

	vec3 ambient = texture(material.diffuse, TexCoord).rgb * light.ambient;

	float diff = max(dot(lightVector, normal), 0.0);
	vec3 diffuse = texture(material.diffuse, TexCoord).rgb * light.diffuse * diff;

	vec3 reflectVector = reflect(-lightVector, normal);
	float spec = pow(max(dot(reflectVector, viewVector), 0.0), material.shininess);
	vec3 specular = texture(material.specular, TexCoord).rgb * light.specular * spec;

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	float intensity = clamp((dot(viewVector, -light.direction) - light.outerCutOff) / (light.innerCutOff - light.outerCutOff), 0.0, 1.0);

	return (ambient + (diffuse + specular) * intensity);
}