#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
out vec3 ourColor;
out vec3 ourNormal;
out vec3 fragPos;
out float visibility;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const float density = 0.04;
const float gradient = 20;

void main() {

	vec4 positionRelativeToCamera = view * model * transform * vec4(aPos, 1.0f);
	gl_Position = projection * positionRelativeToCamera;
	ourColor = vec3(0.5f, 0.5f, 0.7f);
	ourNormal = aNormal;
	fragPos = vec3(model * transform * vec4(aPos, 1.0f));
	float distance = length(positionRelativeToCamera.xyz);
	visibility = clamp(exp(-pow((distance*density), gradient)), 0.0, 1.0);
}
