#version 430
in vec4 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;

out vec4 color;

uniform float materialAmbientIntensity;
uniform float materialSpecularIntensity;
uniform float materialShininess;

layout(std140) uniform GlobalLight {
	vec3 globalLightColor;
	vec3 globalLightPosition;
	float globalLightIntensity;
	float globalLightAttenuationCoef;
};

layout(std140) uniform LocalLight {
	vec3 localLightColor;
	vec3 localLightPosition;
	float localLightIntensity;
	float localLightAttenuationCoef;
};

layout(std140) uniform Skinch {
	float foo;
	vec2 bar;
	vec3 gurt;

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

	vec3 toGlobalLightDir = normalize(globalLightPosition - fragPosition);
	vec4 globalDiffuse = calculateDiffuse(toGlobalLightDir, fragNormal, globalLightIntensity, globalLightColor);

	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec4 specular;
	vec3 toLocalLightDir = normalize(localLightPosition - fragPosition);
	vec4 localDiffuse = calculateDiffuse(toLocalLightDir, fragNormal, localLightIntensity, localLightColor);
	float distanceToLight = length(localLightPosition - fragPosition);
	float attenuation = 1.0 / (1.0 + localLightAttenuationCoef * pow(distanceToLight, 2));
	localDiffuse *= attenuation;
	if(materialShininess > 0.0){
		vec3 reflectDir = reflect(-toLocalLightDir, fragNormal);
		float angIncidence = max(dot(viewDir, reflectDir), 0.0f);
		float spec = pow(angIncidence, materialShininess);
		specular = materialSpecularIntensity * spec * localLightIntensity * vec4(localLightColor, 1.0f); 
	  
		vec3 globalReflectDir = reflect(-toGlobalLightDir, fragNormal);
		angIncidence = max(dot(viewDir, globalReflectDir), 0.0f);
		spec = pow(angIncidence, materialShininess);
		specular += materialSpecularIntensity * spec * globalLightIntensity * vec4(globalLightColor, 1.0f);
		specular.a = 1.0f;
	}

	vec4 texFragColor = texture(tex, fragTexCoord) * fragColor;
	vec4 ambientColor = materialAmbientIntensity * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientColor.a = 1.0f;

	float squrt = 1.0f - gurt.z * gurt.z;
	color = vec4(squrt, squrt, squrt, 1.0f);
	//color = (ambientColor + globalDiffuse + localDiffuse + specular) * texFragColor;
}