#version 430
in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;

out vec4 color;

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
	float attenuation;
};

layout(std140) uniform Lights{
	Light localLight;
	Light globalLight;
};

uniform float materialAmbientIntensity;
uniform vec3 viewPosition;
uniform sampler2D tex;

float quant(float value, int steps) {
	return floor(value * steps) / steps;
}

float edge(vec3 normal, vec3 viewDir, float threshold) {
	float edge = dot(normalize(normal), normalize(viewDir));
	if (0 <= edge && edge < threshold)
		return 0.0; //black outline
	else
		return 1.0; //no outline
}

vec4 diffuse(vec3 lightDir, vec3 unitNormal, float lightIntensity, vec3 lightColor) {
	float cosAngIncidence = clamp(dot(unitNormal, lightDir), 0.0, 1.0);
	cosAngIncidence = quant(cosAngIncidence, 6);
	vec3 diffuse = cosAngIncidence * lightIntensity * lightColor;
	return vec4(diffuse, 1.0f);
}

void main() {

	vec3 viewDir = normalize(viewPosition - fragPosition);

	//difference between global and local light is that local has attenuation falloff
	vec3 toGlobalLightDir = normalize(globalLight.position - fragPosition);
	vec4 globalDiffuse = diffuse(toGlobalLightDir, fragNormal, globalLight.intensity, globalLight.color);

	vec3 toLocalLightDir = normalize(localLight.position - fragPosition);
	vec4 localDiffuse = diffuse(toLocalLightDir, fragNormal, localLight.intensity, localLight.color);
	
	float distanceToLight = length(localLight.position - fragPosition);
	float attenuation = 1.0 / (1.0 + localLight.attenuation * pow(distanceToLight, 2));
	localDiffuse *= attenuation;

	//for now for visibility, keep the ambient above a minimum
	float mai = max(materialAmbientIntensity, 0.2f);

	vec4 texFragColor = texture(tex, fragTexCoord) * fragColor;
	vec4 ambientColor = vec4(mai * vec3(1.0f), 1.0f);
	color = (ambientColor + globalDiffuse + localDiffuse) * texFragColor;
	//color *= edge(-fragNormal, viewDir, 0.4f);
	//color.a = 1.0f; //TODO: edge detection not working rn. fix that
}	