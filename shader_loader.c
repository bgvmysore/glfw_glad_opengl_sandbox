#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "shader_loader.h"


int read_shader_code(char* shdr_src, const char* shdr_file_path) {
	FILE* myfile;
	int f_offset = 0, ret = 0;

	if (shdr_src == NULL) {
		fprintf(stderr, "read_shader_code: Null Pointer Reference\n");
		return -1;
	}

	myfile = fopen(shdr_file_path, "r");
	if (myfile == NULL) {
		fprintf(stderr, "read_shader_code: Failed to open shader file\n");
		return -2;
	}

	do {
		ret = fread(shdr_src + f_offset, 1, 10, myfile);
		f_offset += ret;

		if (ferror(myfile) != 0) {
			fprintf(stderr, "read_shader_code: Failed to read shader file\n");
			return -3;
		}

	} while (feof(myfile) == 0);

	fclose(myfile);
	return 0;
}


GLuint compile_shader(GLenum type, const char* shdr_src) {
	GLuint shader_id;

	shader_id = glCreateShader(type); // Create shader of given type and return its id
	glShaderSource(shader_id, 1, &shdr_src, NULL); // Load the shader src to shader given by id
	glCompileShader(shader_id); // Compile the shader

	/* Error Handling */
	GLint is_compiled = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled); // Get compile status in 'is_compiled' 
	if ((GLboolean)is_compiled == GL_FALSE) {

		GLint msg_len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &msg_len); // Get shader info msg len
		char* msg_buff = malloc(sizeof(char) * msg_len);

		glGetShaderInfoLog(shader_id, msg_len, &msg_len, msg_buff); // Store info msg len to buff

		fprintf(stderr, "%s\n", msg_buff);

		glDeleteShader(shader_id);
		free(msg_buff);

		fprintf(stderr, "compile_shader: Failed to compile shader\n");
		return 0;
	}

	return shader_id;
}


GLuint link_shader_program(int num_of_shaders, ...) {
	GLuint program_id;

	program_id = glCreateProgram();

	/* Loop through va args and attach the shaders specified by their id to shader program */
	va_list shader_id_list;
	va_start(shader_id_list, num_of_shaders);
	for (int i = 0; i < num_of_shaders; i++) {
		GLuint tmp_shader_id = va_arg(shader_id_list, GLuint);
		glAttachShader(program_id, tmp_shader_id);
	}
	va_end(shader_id_list);

	/* Link the program */
	glLinkProgram(program_id);

	/* Error Handling */
	GLint is_linked = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
	if (is_linked == GL_FALSE) {

		GLint msg_len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &msg_len); // Get Program info msg len
		char* msg_buff = malloc(sizeof(char) * msg_len);

		glGetProgramInfoLog(program_id, msg_len, &msg_len, msg_buff); // Store info msg len to buff
		fprintf(stderr, "%s\n", msg_buff);

		fprintf(stderr, "link_shader_program: Failed to link shaders into program\n");

		free(msg_buff);
		glDeleteProgram(program_id);
		return 0;
	}

	/* Loop through va args to detach and delete shaders after succesful compilation */
	va_start(shader_id_list, num_of_shaders);
	for (int i = 0; i < num_of_shaders; i++) {
		GLuint tmp_shader_id = va_arg(shader_id_list, GLuint);
		glDeleteShader(tmp_shader_id);
	}
	va_end(shader_id_list);

	return program_id;
}
