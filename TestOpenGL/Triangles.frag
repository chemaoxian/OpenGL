#version 430 core

layout (location = 0) out vec4 fColor;

in vec4 color;
in vec2 texture;

void main() {
     fColor = texture2D(texture0, texture);
}