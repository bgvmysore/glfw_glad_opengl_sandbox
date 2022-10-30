#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

#include <glad/glad.h>

/* Min and Mag filter to `Nearest Approximation' interpolation
   Clamp texture in x and y axis */
#define TEX2D_DEFAULT_CONFIG {               \
		GL_TEXTURE_MIN_FILTER, GL_NEAREST,   \
		GL_TEXTURE_MAG_FILTER, GL_NEAREST,   \
		GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE, \
		GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE, \
        GL_NONE                              \
	}


/* This functions takes a path to png image file and an array of texture parameters and
   returns a texture id on successful execution and 0 if it fails

path_to_png : path to png file as const char*
tex_params  : an GLint array of parameter key and parameter value which will be ended by GL_NONE
              Example:{ TEX_PARAM_1, TEX_PARAM_1_VALUE
                        TEX_PARAM_2, TEX_PARAM_2_VALUE
                        TEX_PARAM_3, TEX_PARAM_3_VALUE
                        GL_NONE
                      }
flip_flag   : Flips the loaded image if this flag is 1 (some image formats requires this Ex: PNG)

Return value: texture id to the texture generated is retured which is GLuint
              On Errors this returns 0
*/
GLuint tex2d_load_from_image(const char* path_to_png, const GLint* tex_params, int flip_flag);

/* Binds the texture given by `tex_id' to the required `tex_slot' */
void tex2d_bind(GLuint tex_id, GLenum tex_slot);

/* Unbinds the texture which is currently bound */
void tex2d_unbind(void);

/* Deletes the texture. This frees up the texture buffer in the GPU memory */
void tex2d_cleanup(const GLuint* tex_id);

#endif //_TEXTURE_LOADER_H_