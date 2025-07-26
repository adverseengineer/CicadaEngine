#version 430

in vec3 fragNormal;

out vec4 color;

void main() {
	
	vec3 temp = normalize(fragNormal);
	color = vec4(temp.x, temp.y, temp.z, 1);
}