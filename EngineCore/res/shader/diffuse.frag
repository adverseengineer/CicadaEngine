#version 430
in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;

out vec4 color;

uniform float materialAmbientIntensity;
uniform float materialSpecularIntensity;
uniform float materialShininess;

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
	float attenuation;
};

layout(std140) uniform Lights {
	Light localLight;
	Light globalLight;
};

uniform vec3 viewPosition;

uniform sampler2D tex;

vec4 calculateDiffuse(vec3 lightDir, vec3 unitNormal, float lightIntensity, vec3 lightColor) {
	float cosAngIncidence = dot(unitNormal, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0.0f, 1.0f);
	vec3 diffuse = cosAngIncidence * lightIntensity * lightColor;
	return vec4(diffuse, 1.0f);
}

void main() {

	vec3 toGlobalLightDir = normalize(globalLight.position - fragPosition);
	vec4 globalDiffuse = calculateDiffuse(toGlobalLightDir, fragNormal, globalLight.intensity, globalLight.color);

	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec4 specular;
	vec3 toLocalLightDir = normalize(localLight.position - fragPosition);
	vec4 localDiffuse = calculateDiffuse(toLocalLightDir, fragNormal, localLight.intensity, localLight.color);
	float distanceToLight = length(localLight.position - fragPosition);
	float attenuation = 1.0 / (1.0 + localLight.attenuation * pow(distanceToLight, 2));
	localDiffuse *= attenuation;
	if(materialShininess > 0.0){
		vec3 reflectDir = reflect(-toLocalLightDir, fragNormal);
		float angIncidence = max(dot(viewDir, reflectDir), 0.0f);
		float spec = pow(angIncidence, materialShininess);
		specular = materialSpecularIntensity * spec * localLight.intensity * vec4(localLight.color, 1.0f); 
	  
		vec3 globalReflectDir = reflect(-toGlobalLightDir, fragNormal);
		angIncidence = max(dot(viewDir, globalReflectDir), 0.0f);
		spec = pow(angIncidence, materialShininess);
		specular += materialSpecularIntensity * spec * globalLight.intensity * vec4(globalLight.color, 1.0f);
		specular.a = 1.0f;
	}

	vec4 texFragColor = texture(tex, fragTexCoord) * fragColor;
	vec4 ambientColor = materialAmbientIntensity * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientColor.a = 1.0f;

	color = (ambientColor + globalDiffuse + localDiffuse + specular) * texFragColor;
}