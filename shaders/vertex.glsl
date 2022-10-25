// Vertex Shader

layout (location = 0) in vec3 attPos;
layout (location = 1) out vec3 attCol;

void main() {
	gl_Position = vec4(attPos, 1.0); 
}
