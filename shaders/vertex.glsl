// Vertex Shader
#version 300 es

layout (location = 0) in vec3 att_pos;
layout (location = 1) in vec3 att_color;
layout (location = 2) in vec2 att_tex;

out vec3 vert_color;
out vec2 frg_tex_coord;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 mod_mat;

void main() {
	gl_Position = proj_mat * view_mat * mod_mat * vec4(att_pos, 1.0);
	vert_color = att_color;
	frg_tex_coord = att_tex;
}
