#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cglm/cglm.h>

#include "shader_loader.h"

#define NUM_VERTICES 4
#define NUM_INDICES  6

struct vertex {
	/* Positions */
	GLfloat x, y, z;

	/* Color */
	GLfloat r, g, b;

	/* Texture */
	GLfloat u, v;
};

const struct vertex triangle_vert_data[NUM_VERTICES] = {
	{.x = -0.5, .y = -0.5, .z = 0, .r = 0.01, .g = 0.34, .b = 0.56, .u = 0.0, .v = 0.0}, // 0 Q-III
	{.x =  0.5, .y = -0.5, .z = 0, .r = 0.38, .g = 0.70, .b = 0.50, .u = 1.0, .v = 0.0}, // 1 Q-IV
	{.x =  0.5, .y =  0.5, .z = 0, .r = 0.77, .g = 0.80, .b = 0.42, .u = 1.0, .v = 1.0}, // 2 Q-I
	{.x = -0.5, .y =  0.5, .z = 0, .r = 0.87, .g = 0.25, .b = 0.35, .u = 0.0, .v = 1.0}  // 3 Q-II
};

const GLubyte index_data[NUM_INDICES] = {
	0, 1, 2,
	2, 3, 0
};

char vertx_shader_buff[1024] = {0};
char fragt_shader_buff[1024] = {0};

int main(void) {
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit()) return -1;

	/* Create a Glfw Window */
	window = glfwCreateWindow(600, 600, "Triangle", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make glfw window current GL Context */
	glfwMakeContextCurrent(window);

	/* Initialize Glad with Glfw */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		return -1;
	}

	GLuint vert_arr_id, vert_buff_id, element_buff_id;

	/* Vertex Arrays and Vertex Buffers */
	glGenVertexArrays(1, &vert_arr_id);    // Generate one Vertex array and assign its id to 'vert_arr_id'
	glBindVertexArray(vert_arr_id);        // Bind the vertex array given by 'vert_arr_id'

	glGenBuffers(1, &vert_buff_id); // Generate buffer and assign to 'vert_buff_if'

	// Note: Make sure that the required Vertex Array is Bound before binding Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id); // Bind the buffer given by 'vert_buff_id'

	// Assign Data to given buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * NUM_VERTICES, triangle_vert_data, GL_STATIC_DRAW);

	// Describe Vertex Attrib Pointer or vertex layout
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*) offsetof(struct vertex, x)); // index 0 - position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*) offsetof(struct vertex, r)); // index 1 - colors
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*) offsetof(struct vertex, u)); // index 2 - texture

	glEnableVertexAttribArray(0); // Enable index 0 or position Attrib Pntr
	glEnableVertexAttribArray(1); // Enable index 1 or Color Attrib Pntr
	glEnableVertexAttribArray(2); // Enable index 2 or Texture Attrib Pntr

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Vertex Buffer

	glBindVertexArray(0);                  // Unbind Vertex Array

	/* Elements Buffers */
	glGenBuffers(1, &element_buff_id); // Generate a buffer id

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buff_id); // Bind that buffer given 'by element_buff_id' as element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * NUM_INDICES, index_data, GL_STATIC_DRAW); // Assign indices data to element buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind elements buffer Note: Bind this required Elements Array while drawing draw elements

	/* Textures */
	GLuint texture_id;
	stbi_uc* tex_image_buffer = NULL;
	int tex_image_width, tex_image_height, tex_image_nchanels;

	stbi_set_flip_vertically_on_load(1); // Flip image vertically as png image requires this

	// Load Image and get an handle to it
	tex_image_buffer = stbi_load("resources/textures/apple_sprite_640px.png", &tex_image_width, &tex_image_height, &tex_image_nchanels, 4);
	if (tex_image_buffer == NULL) {
		fprintf(stderr, "Failed to load texture image file\n");
	}

	glGenTextures(1, &texture_id); // Generate 1 texter buffer and store its id in 'texture_id'
	glBindTexture(GL_TEXTURE_2D, texture_id); // Bind the given texture

	// Set Texture Buffer Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Min and Mag filter to Nearest Approximation interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Clamp texture in x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Clamp texture in y axis

	// Set texture image buffer on GPU and load pixel data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_image_width, tex_image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_image_buffer);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

	stbi_image_free(tex_image_buffer); // Free the image buffer as it is already in GPU


	/* GL Shaders*/
	/* Load shader sources on to thier respective char buffers from file path specified */
	read_shader_code(vertx_shader_buff, "shaders/vertex.glsl");
	read_shader_code(fragt_shader_buff, "shaders/fragment.glsl");

	GLuint vertx_shader_id = compile_shader(GL_VERTEX_SHADER, vertx_shader_buff); // compile shader with specificed shader type and src
	GLuint fragt_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragt_shader_buff);
	GLuint shader_program_id = link_shader_program(2, vertx_shader_id, fragt_shader_id); // Link specified shaders into a shader program

	glUseProgram(shader_program_id); // Use the specified shader program using its id

	/* Main Program Loop */
	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.33f, 0.37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vert_arr_id); // Bind the required Vertex Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buff_id); // Bind the required elements or indices array buffer

		// Bind Texture
		glActiveTexture(GL_TEXTURE0); // Bind required texture slot
		glBindTexture(GL_TEXTURE_2D, texture_id); // Bind texture given by id

		// Get loaction of the sampler slot and give it slot info
		// in our case we have GL_TEXTURE0 so we give that uniform a value of 0
		glUniform1i(glGetUniformLocation(shader_program_id, "tex_sampler_slot"), 0);

		glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_BYTE, 0); // Using glDrawElements instead glDrawArrays

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* Clean Up */
	glDeleteProgram(shader_program_id); // Delete Shader Program
	glfwDestroyWindow(window); // Destroy window object

	glfwTerminate(); // Terminate GLFW

	return 0;
}

