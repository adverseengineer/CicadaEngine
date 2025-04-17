#version 430
in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;

out vec4 color;

uniform float materialAmbientIntensity;
//uniform float materialSpecularIntensity;
//uniform float materialShininess;

//layout(std140, binding = 1) uniform LightData {
	uniform vec3 globalLightPosition;
	uniform vec3 globalLightColor;
	uniform float globalLightIntensity;
	uniform float globalLightAttenuationCoef;

	uniform vec3 localLightPosition;
	uniform vec3 localLightColor;
	uniform float localLightIntensity;
	uniform float localLightAttenuationCoef;
//};

uniform vec3 viewPosition;

uniform sampler2D tex;

float quant(float value, int steps) {
	return floor(value * steps) / steps;
}

vec4 diffuse(vec3 lightDir, vec3 unitNormal, float lightIntensity, vec3 lightColor) {
	float cosAngIncidence = clamp(dot(unitNormal, lightDir), 0.0, 1.0);
	cosAngIncidence = quant(cosAngIncidence, 6);
	vec3 diffuse = cosAngIncidence * lightIntensity * lightColor;
	return vec4(diffuse, 1.0f);
}

vec3 edgeDetect(vec3 normal, vec3 viewDir, float threshold) {
	float edge = dot(normalize(normal), normalize(viewDir));
	if (edge < threshold) {
		return vec3(0.0); //black outline
	} else {
		return vec3(1.0); //no outline
	}
}

void main() {

	vec3 viewDir = normalize(viewPosition - fragPosition);

	//difference between global and local light is that local has attenuation falloff
	vec3 toGlobalLightDir = normalize(globalLightPosition - fragPosition);
	vec4 globalDiffuse = diffuse(toGlobalLightDir, fragNormal, globalLightIntensity, globalLightColor);

	vec3 toLocalLightDir = normalize(localLightPosition - fragPosition);
	vec4 localDiffuse = diffuse(toLocalLightDir, fragNormal, localLightIntensity, localLightColor);
	
	float distanceToLight = length(localLightPosition - fragPosition);
	float attenuation = 1.0 / (1.0 + localLightAttenuationCoef * pow(distanceToLight, 2));
	localDiffuse *= attenuation;

	//for now for visibility, keep the ambient above a minimum
	float mai = max(materialAmbientIntensity, 0.2f);

	vec4 texFragColor = texture(tex, fragTexCoord) * fragColor;
	vec4 ambientColor = vec4(mai * vec3(1.0f, 1.0f, 1.0f), 1.0f);
	color = (ambientColor + globalDiffuse + localDiffuse) * texFragColor;
	//color *= vec4(edgeDetect(fragNormal, viewDir, 0.0f), 1.0f);
}