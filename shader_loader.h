#ifndef _SHADER_LOADER_H_
#define _SHADER_LOADER_H_

/* Read the shader code of a file given by 'shdr_file_path' and
   save it in 'shdr_src' char pointer */
int read_shader_code(char* shdr_src, const char* shdr_file_path);

#endif //_SHADER_LOADER_H_
