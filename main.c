#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_loader.h"

#define NUM_VERTICES 3

struct vertex {
	/* Positions */
	GLfloat x,y,z;

	/* Color */
	GLfloat r,g,b;
};

const struct vertex triangle_vert_data[NUM_VERTICES] = {
	{.x = -0.5, .y = -0.5, .z = 0, .r = 1.0, .g = 0.0, .b = 0.0},
	{.x =  0.5, .y = -0.5, .z = 0, .r = 0.0, .g = 1.0, .b = 0.0},
	{.x =  0.0, .y =  0.5, .z = 0, .r = 0.0, .g = 0.0, .b = 1.0}
};

char vertx_shader_buff[1024] = {0};
char fragt_shader_buff[1024] = {0};

int main(void) {
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit()) return -1;

	/* Create a Glfw Window */
	window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
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

	GLuint vert_arr_id, vert_buff_id;

	glGenVertexArrays(1, &vert_arr_id);    // Generate one Vertex array and assign its id to 'vert_arr_id'
	glBindVertexArray(vert_arr_id);        // Bind the vertex array given by 'vert_arr_id'

	glGenBuffers(1, &vert_buff_id); // Generate buffer and assign to 'vert_buff_if'
	glBindBuffer(GL_ARRAY_BUFFER, vert_buff_id); // Bind the buffer given by 'vert_buff_id'

	// Assign Data to given buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * NUM_VERTICES, triangle_vert_data, GL_STATIC_DRAW);

	// Describe Vertex Attrib Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) offsetof(struct vertex, x)); // index 0 - position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) offsetof(struct vertex, r)); // index 1 - colors

	glEnableVertexAttribArray(0); // Enable index 0 or position Attrib Pntr
	glEnableVertexAttribArray(1); // Enable index 1 or Color Attrib Pntr

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind Buffer 

	glBindVertexArray(0);                 // Unbind Vertex array

	// Load shaders on to thier char buffer
	read_shader_code(vertx_shader_buff, "shaders/vertex.glsl");
	read_shader_code(fragt_shader_buff, "shaders/fragment.glsl");

	GLuint vertx_shader_id = compile_shader(GL_VERTEX_SHADER, vertx_shader_buff);
	GLuint fragt_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragt_shader_buff);

	GLuint shader_program_id = link_shader_program(2, vertx_shader_id, fragt_shader_id);
	glUseProgram(shader_program_id);

	/* Main Program Loop */
	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vert_arr_id); // Bind the required Vertex Array
		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle primitive form bounded vertex array

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_program_id);

	/* Terminate GLFW */
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

