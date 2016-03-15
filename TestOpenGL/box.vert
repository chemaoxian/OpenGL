#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec4 vNormal;
layout(location = 3) in vec2 vUV;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec4 color;
out vec2 out_uv;

void main() {
     gl_Position = projectMatrix * viewMatrix * worldMatrix * vPosition;
     color = clamp(vPosition, 0.0, 1.0);
	 out_uv = vUV;
}