// Vertex Shader
#version 300 es

layout (location = 0) in vec3 att_pos;
layout (location = 1) in vec3 att_color;
layout (location = 2) in vec2 att_tex;

out vec3 vert_color;
out vec2 frg_tex_coord;

void main() {
	gl_Position = vec4(att_pos, 1.0);
	vert_color = att_color;
	frg_tex_coord = att_tex;
}
