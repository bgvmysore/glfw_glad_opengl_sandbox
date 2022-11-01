#ifndef _VERTICES_H_
#define _VERTICES_H_

#include <glad/glad.h>
#include <stddef.h>

typedef struct {
	GLenum  type;
	GLubyte count;
} vertex_layout_element_t;

/* Vertex Buffer Object */

/*
Create a vertex buffer object and return its id.
This takes a pointer to float array and no of floats in that array and also the GL render type
*/
GLuint vertices_vbo_create(const GLfloat vertex_array[], GLuint array_len, GLenum vertices_usage);

/* Binds the VBO of given id*/
void vertices_vbo_bind(GLuint vertex_buffer_id);

/* Unbinds the currently bound VBO */
void vertices_vbo_unbind(void);

/* Deletes the VBO of given VBO id*/
void vertices_vbo_cleanup(GLuint vertex_buffer_id);


/* Vertex Array Object */

/*
Creates a Vertex array object and returns its id.
VAO takes primarily two things VBO and Layout of vertices in that VBO.
VBO id is given as first argument
vertex_layout_element_t consists of two values GL value enum and no of such elements.
An array of such vertex_layout_elements and num of elements is given to this function.
*/
GLuint vertices_vao_create(GLuint vbo_id, const vertex_layout_element_t* vertex_buffer_layout, size_t num_elements_layout);

/* Bind VAO of given id */
void vertices_vao_bind(GLuint vertex_array_id);

/* Unbind the currently Bound VAO */
void vertices_vao_unbind(void);

/* Deletes the VAO of given VAO id*/
void vertices_vao_cleanup(GLuint vertex_array_id);


/* Element Buffer Object */

/*
Create a elements buffer object and return its id.
This takes a pointer to unsigned int array and no of unsinged ints in that array and also the GL render type
*/
GLuint vertices_ebo_create(const GLuint* indices_array, GLuint array_len, GLenum element_buff_usage);

/* Bind EBO of given id */
void vertices_ebo_bind(GLuint elements_buff_id);

/* Unbind the currently Bound EBO */
void vertices_ebo_unbind(void);

/* Deletes the EBO of given EBO id*/
void vertices_ebo_cleanup(GLuint elements_buffer_id);

#endif //_VERTICES_H_