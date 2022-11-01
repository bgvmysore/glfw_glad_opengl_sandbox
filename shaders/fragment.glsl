// Fragment Shader
#version 300 es

precision mediump float;

uniform sampler2D tex_sampler_slot;
in vec3 vert_color;
in vec2 frg_tex_coord;

layout(location = 0) out vec4 frag_color;

void main() {
	vec4 tex_data = texture(tex_sampler_slot, frg_tex_coord);
	/*  frag_color = vec4(tex_data.r + ((1.0f - tex_data.a) * vert_color.r),
	 				  tex_data.g + ((1.0f - tex_data.a) * vert_color.g),
	 				  tex_data.b + ((1.0f - tex_data.a) * vert_color.b),
					  1.0f); */
	frag_color = tex_data;
}
