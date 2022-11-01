#include "vertices.h"


#define NULL ((void *)0)


void vertices_vbo_layout(const vertex_layout_element_t* buffer_layout, size_t n);

size_t vertices_helper_get_sizeof_type(GLenum type);

size_t vertices_helper_layout_offset_at(const vertex_layout_element_t* buffer_layout, unsigned index);


/* Vertex Buffer Object */


GLuint vertices_vbo_create(const GLfloat vertex_array[], GLuint array_len, GLenum vertices_usage) {
    GLuint vertex_buffer_id = 0;

    glGenBuffers(1, &vertex_buffer_id); // Generate buffer and assign to 'vertex_buffer_id'

    // Note: Make sure that the required Vertex Array is Bound before binding Vertex Buffer
    vertices_vbo_bind(vertex_buffer_id);

    // Assign Data to given buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * array_len, vertex_array, vertices_usage);

    vertices_vbo_unbind(); // Unbind vertex buffer
    return vertex_buffer_id;
}


void vertices_vbo_bind(GLuint vertex_buffer_id) {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id); // Bind the buffer
}


void vertices_vbo_unbind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}


void vertices_vbo_cleanup(GLuint vertex_buff_id) {
    if (vertex_buff_id != GL_NONE) {
        glDeleteBuffers(1, &vertex_buff_id);
    }
}

void vertices_vbo_layout(const vertex_layout_element_t* buffer_layout, size_t nums) {
    size_t stride_of_layout = 0;
    for(size_t i = 0; i < nums; i++) {
        stride_of_layout += buffer_layout[i].count * vertices_helper_get_sizeof_type(buffer_layout[i].type);
    }

    // Set Vertex Attribute Pointer info
    for (size_t i = 0; i < nums; i++) {
        glVertexAttribPointer(i, buffer_layout[i].count, buffer_layout[i].type, GL_FALSE, stride_of_layout,
        (const void*) (vertices_helper_layout_offset_at(buffer_layout, i)));
    }

    // Enable all indexes or Attrib Pntrs
    for (size_t i = 0; i < nums; i++) {
        glEnableVertexAttribArray(i);
    }
}


size_t vertices_helper_layout_offset_at(const vertex_layout_element_t* buffer_layout, unsigned index) {
    size_t ret = 0;
    for (unsigned i = 0; i < index; i++) {
        ret +=  vertices_helper_get_sizeof_type(buffer_layout[i].type) * buffer_layout[i].count;
    }
    return ret;
}


size_t vertices_helper_get_sizeof_type(GLenum type) {
    size_t ret_size = 0;
    switch (type) {
        case GL_BYTE:
            ret_size = sizeof(GLbyte); break;

        case GL_UNSIGNED_BYTE:
            ret_size = sizeof(GLubyte); break;

        case GL_INT:
            ret_size = sizeof(GLint); break;

        case GL_UNSIGNED_INT:
            ret_size = sizeof(GLuint); break;

        case GL_FLOAT:
            ret_size = sizeof(GLfloat); break;

        case GL_DOUBLE:
            ret_size =  sizeof(GLdouble); break;

        default:
            ret_size = 0; break;
    }
    return ret_size;
}


/* Vertex Array Object */


GLuint vertices_vao_create(GLuint vbo_id, const vertex_layout_element_t* vertex_buffer_layout, size_t num_elements_layout) {
    GLuint vertex_array_id = 0;
    glGenVertexArrays(1, &vertex_array_id); // Generate one Vertex array and assign its id to 'vertex_array_id'
    if (vertex_array_id == 0) {
        return 0; // Simple error handeling
    }

    vertices_vao_bind(vertex_array_id); // Binding VAO first
    vertices_vbo_bind(vbo_id); // Next Binding required VBO

    // Bind the layout
    vertices_vbo_layout(vertex_buffer_layout, num_elements_layout);

    vertices_vao_unbind();
    vertices_vbo_unbind();
    return vertex_array_id;
}


void vertices_vao_bind(GLuint vertex_array_id) {
    glBindVertexArray(vertex_array_id);
}


void vertices_vao_unbind(void) {
    glBindVertexArray(GL_NONE);
}


void vertices_vao_cleanup(GLuint vertex_array_id) {
    if (vertex_array_id != GL_NONE) {
        glDeleteVertexArrays(1, &vertex_array_id);
    }
}


/* Elements Buffer Object */


GLuint vertices_ebo_create(const GLuint* indices_array, GLuint array_len, GLenum element_buff_usage) {
    GLuint element_buff_id;

    glGenBuffers(1, &element_buff_id); // Generate a buffer id
    vertices_ebo_bind(element_buff_id); // Bind that buffer given 'by element_buff_id' as element buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * array_len, indices_array, element_buff_usage); // Assign indices data to element buffer

    vertices_ebo_unbind();
    return element_buff_id;
}


void vertices_ebo_bind(GLuint elements_buff_id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements_buff_id); // Bind the buffer
}


void vertices_ebo_unbind(void) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
}


void vertices_ebo_cleanup(GLuint elements_buffer_id) {
    if (elements_buffer_id != GL_NONE) {
        glDeleteBuffers(1, &elements_buffer_id);
    }
}
