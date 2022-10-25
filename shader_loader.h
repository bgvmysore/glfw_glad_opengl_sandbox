#ifndef _SHADER_LOADER_H_
#define _SHADER_LOADER_H_

#include <glad/glad.h>

/* Read the shader code of a file given by 'shdr_file_path' and
   save it in 'shdr_src' char pointer */
int read_shader_code(char* shdr_src, const char* shdr_file_path);

/* Compile the shader src given buf char buff of specified type */
GLuint compile_shader(GLenum type, const char* shdr_src);

/* link the given number of shader into shader program and return its id */
GLuint link_shader_program(int num_of_shaders, ...);

#endif //_SHADER_LOADER_H_
