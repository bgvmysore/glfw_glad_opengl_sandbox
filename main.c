#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_loader.h"

#define NUM_VERTICES 4
#define NUM_INDICES  6

struct vertex {
	/* Positions */
	GLfloat x,y,z;

	/* Color */
	GLfloat r,g,b;
};

const struct vertex triangle_vert_data[NUM_VERTICES] = {
	{.x = -0.5, .y = -0.5, .z = 0, .r = 0.01, .g = 0.34, .b = 0.56}, // 0 Q-III
	{.x =  0.5, .y = -0.5, .z = 0, .r = 0.38, .g = 0.70, .b = 0.50}, // 1 Q-IV
	{.x =  0.5, .y =  0.5, .z = 0, .r = 0.77, .g = 0.80, .b = 0.42}, // 2 Q-I
	{.x = -0.5, .y =  0.5, .z = 0, .r = 0.87, .g = 0.25, .b = 0.35}  // 3 Q-II
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) offsetof(struct vertex, x)); // index 0 - position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) offsetof(struct vertex, r)); // index 1 - colors

	glEnableVertexAttribArray(0); // Enable index 0 or position Attrib Pntr
	glEnableVertexAttribArray(1); // Enable index 1 or Color Attrib Pntr

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Vertex Buffer

	glBindVertexArray(0);                  // Unbind Vertex Array

	/* Elements Buffers */
	glGenBuffers(1, &element_buff_id); // Generate a buffer id

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buff_id); // Bind that buffer given 'by element_buff_id' as element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * NUM_INDICES, index_data, GL_STATIC_DRAW); // Assign indices data to element buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind elements buffer Note: Bind this required Elements Array while drawing draw elements

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
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vert_arr_id); // Bind the required Vertex Array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buff_id); // Bind the required elements or indices array buffer

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

