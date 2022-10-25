// Fragment Shader
precision mediump float;

in vec3 attCol;

void main() {
	gl_FragColor = vec4(attCol, 1.0f);
}
