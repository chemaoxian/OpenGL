#version 430 core

out vec4 fColor;

in vec4 color;
in vec2 out_uv;

uniform sampler2D tex;

void main() {
     vec4 temp = texture(tex, out_uv); //+ vec4(0.1, 0.1, 0.1, 1.0);
	 //float gray = temp.x * 0.33 + temp.y * 0.33 + temp.z * 0.33;
	// fColor = vec4(gray, gray, gray, 1.0);
	fColor = temp;
}