#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shader_loader.h"


int read_shader_code(char* shdr_src, const char* shdr_file_path) {
	FILE* myfile;
	
	if (shdr_src == NULL) {
		fprintf(stderr, "read_shader_code: Null Pointer Reference\n");
		return -1;
	}
	
	myfile = fopen(shdr_file_path, "r");
	if (myfile == NULL) {
		fprintf(stderr, "read_shader_code: Failed to open shader file\n");
		return -2;
	}
	
	fseek(myfile, 0, SEEK_END); // goto end of file
	
	
	
	fclose(myfile);
	return 0;
}
