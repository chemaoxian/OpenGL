#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 uv;

out vec4 color;
out vec2 texture;

void main() {
     gl_Position = vPosition;
	 texture = uv;
}