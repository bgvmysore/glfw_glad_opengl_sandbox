#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture_loader.h"


static void tex2d_set_params(const GLint* params_array);


GLuint tex2d_load_from_image(const char* _path_to_png, const GLint* tex_params, int flip_flag) {
    stbi_uc* image_buffer = NULL;
    int image_width = 0, image_height = 0, image_nchanels = 0;
    GLuint tex_id = 0;

    stbi_set_flip_vertically_on_load(flip_flag); // Flip image vertically if flip_flag is 1

    image_buffer = stbi_load(_path_to_png, &image_width, &image_height, &image_nchanels, 4);
    if (image_buffer == NULL) {
        fprintf(stderr, "Failed to load texture image file\n");
        return 0;
    }

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id); // Bind the given texture

    //Set texture parameters
    tex2d_set_params(tex_params);

    // Set texture image buffer on GPU and load pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

    stbi_image_free(image_buffer);
    return tex_id;
}


void tex2d_bind(GLuint tex_id, GLenum tex_slot) {
    glActiveTexture(tex_slot);
    glBindTexture(GL_TEXTURE_2D, tex_id);
}


void tex2d_unbind(void) {
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void tex2d_cleanup(const GLuint* tex_id) {
    if (*tex_id != 0 && tex_id != NULL)
        glDeleteTextures(1, tex_id);
}


static void tex2d_set_params(const GLint* params_array) {
    GLuint i = 0;
    while (params_array[i] != GL_NONE) {
        glTexParameteri(GL_TEXTURE_2D, params_array[i], params_array[i+1]);
        i += 2;
    }
}