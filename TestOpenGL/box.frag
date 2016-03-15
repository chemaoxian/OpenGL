#version 430 core

layout (location = 0) out vec4 fColor;

in vec4 color;
in vec2 out_uv;

uniform sampler2D tex;

void main() {
     //vec4 temp = texture(tex, out_uv); //+ vec4(0.1, 0.1, 0.1, 1.0);
	float grey = dot(texture2D(tex, out_uv).rgb, vec3(0.299, 0.587, 0.114));
  
    fColor = vec4(grey, grey, grey, 1.0);
}