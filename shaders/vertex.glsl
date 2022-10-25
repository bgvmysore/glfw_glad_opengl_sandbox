// Vertex Shader
#version 300 es

layout (location = 0) in vec3 attPos;
layout (location = 1) in vec3 attColor;
out vec3 color;

void main() {
	gl_Position = vec4(attPos, 1.0); 
	color = attColor;
}
