#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "texture_loader.h"
#include "shader_loader.h"
#include "vertices.h"


#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define NUM_VERTICES 4
#define NUM_INDICES  6
#define NUM_VERT_ATTRIB 3


static void basic_gl_init_configs(void);
static void basic_gl_release_configs(void);


struct vertex {
	/* Positions */
	GLfloat x, y, z;

	/* Color */
	GLfloat r, g, b;

	/* Texture */
	GLfloat u, v;
};

const struct vertex vert_data[NUM_VERTICES] = {
	{.x = -0.5, .y = -0.5, .z = 0, .r = 0.01, .g = 0.34, .b = 0.56, .u = 0.0, .v = 0.0}, // 0 Q-III
	{.x =  0.5, .y = -0.5, .z = 0, .r = 0.38, .g = 0.70, .b = 0.50, .u = 1.0, .v = 0.0}, // 1 Q-IV
	{.x =  0.5, .y =  0.5, .z = 0, .r = 0.77, .g = 0.80, .b = 0.42, .u = 1.0, .v = 1.0}, // 2 Q-I
	{.x = -0.5, .y =  0.5, .z = 0, .r = 0.87, .g = 0.25, .b = 0.35, .u = 0.0, .v = 1.0}  // 3 Q-II
};

const vertex_layout_element_t vert_layout[NUM_VERT_ATTRIB] = {
	{GL_FLOAT, 3},
	{GL_FLOAT, 3},
	{GL_FLOAT, 2}
};

const GLuint index_data[NUM_INDICES] = {
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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Sandbox", NULL, NULL);
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


	basic_gl_init_configs();


	/* Vertex Buffer, Vertex Array and Elements Buffer */
	GLuint vert_arr_id, vert_buff_id, element_buff_id;
	vert_buff_id = vertices_vbo_create((const GLfloat*)vert_data, NUM_VERTICES * sizeof(struct vertex), GL_STATIC_DRAW);
	vert_arr_id = vertices_vao_create(vert_buff_id, vert_layout, NUM_VERT_ATTRIB);
	element_buff_id = vertices_ebo_create(index_data, NUM_INDICES, GL_STATIC_DRAW);


	/* Textures */
	GLuint tex_mcface_id, tex_apple_id, tex_eye_id;
	GLint tex_parameters[9] = TEX2D_DEFAULT_CONFIG;
	tex_mcface_id = tex2d_load_from_image("resources/textures/mc_face_640px.png", tex_parameters, 1);
	tex_apple_id = tex2d_load_from_image("resources/textures/apple_sprite_640px.png", tex_parameters, 1);
	tex_eye_id = tex2d_load_from_image("resources/textures/eye_640px.png", tex_parameters, 1);


	/* GL Shaders*/
	/* Load shader sources on to thier respective char buffers from file path specified */
	read_shader_code(vertx_shader_buff, "shaders/vertex.glsl");
	read_shader_code(fragt_shader_buff, "shaders/fragment.glsl");
	GLuint vertx_shader_id = compile_shader(GL_VERTEX_SHADER, vertx_shader_buff); // compile shader with specificed shader type and src
	GLuint fragt_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragt_shader_buff);
	GLuint shader_program_id = link_shader_program(2, vertx_shader_id, fragt_shader_id); // Link specified shaders into a shader program
	glUseProgram(shader_program_id); // Use the specified shader program using its id

	/* Get Uniform Locations for required uniforms */
	GLint loc_proj_mat, loc_view_mat, loc_mod_mat, loc_tex_sampler_slot;
	loc_proj_mat = glGetUniformLocation(shader_program_id, "proj_mat");
	loc_view_mat = glGetUniformLocation(shader_program_id, "view_mat");
	loc_mod_mat = glGetUniformLocation(shader_program_id, "mod_mat");
	loc_tex_sampler_slot = glGetUniformLocation(shader_program_id, "tex_sampler_slot");

	/* Creating Projection Matrix Ortho (2D) */
	mat4 projection_mat = GLM_MAT4_IDENTITY_INIT;
	GLfloat window_aspect_ratio = (GLfloat) WINDOW_WIDTH/ (GLfloat) WINDOW_HEIGHT;
	glm_ortho(-1 * window_aspect_ratio, window_aspect_ratio, -1, 1, -1, 1, projection_mat);

	// Setting Uniform, Note this should be done after binding shader program
	glUniformMatrix4fv(loc_proj_mat, 1, GL_FALSE, (GLfloat*)projection_mat);

	/* Creating View or Camera Matrix */
	mat4 camera_mat = GLM_MAT4_IDENTITY_INIT;
	glm_translate(camera_mat, (vec3){0.125f, 0.3f, 0.0f});
	glUniformMatrix4fv(loc_view_mat, 1, GL_FALSE, (GLfloat*)camera_mat);

	/* Creating Model Matrix */
	mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
	glUniformMatrix4fv(loc_mod_mat, 1, GL_FALSE, (GLfloat*)model_mat);

	// Bind Texture
	// Note: Texture should be bound only after binding the shader program
	tex2d_bind(tex_mcface_id, GL_TEXTURE0);
	tex2d_bind(tex_apple_id, GL_TEXTURE1);
	tex2d_bind(tex_eye_id, GL_TEXTURE2);

	/* Main Program Loop */
	double mouse_x, mouse_y;
	float trans_vec2[2] = {0};
	float time_0 = glfwGetTime(), time_1;
	float fps_frams = 0;
	short tex_sampler_slot = 0;
	while(!glfwWindowShouldClose(window)) {

		glClearColor(0.3f, 0.33f, 0.37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vertices_vao_bind(vert_arr_id); // Bind the required Vertex Array
		vertices_ebo_bind(element_buff_id); // Bind the required elements or indices array buffer

		// Give texture slot info
		glUniform1i(loc_tex_sampler_slot, tex_sampler_slot);

		// Set value for camera matrix uniform
		glUniformMatrix4fv(loc_view_mat, 1, GL_FALSE, (GLfloat*)camera_mat);

		glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, 0); // Using glDrawElements instead glDrawArrays

		glfwSwapBuffers(window);
		glfwPollEvents();

		/* FPS Calculation */
		fps_frams += 1.0f;
		time_1 = glfwGetTime();
		if (time_1 - time_0 > 5) {
			printf("FPS: %f \n", fps_frams/5);
			fps_frams = 0.0f;
			time_0 = time_1;

			printf("Mouse Norm: ( %f , %f ) \n", trans_vec2[0], trans_vec2[1]);
			tex_sampler_slot++;
			if (tex_sampler_slot >= 3) tex_sampler_slot = 0;
		}

		/* Object Track Mouse */
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			glfwGetCursorPos(window, &mouse_x, &mouse_y);
			if(mouse_x >= WINDOW_WIDTH) mouse_x = (double) WINDOW_WIDTH;
			if(mouse_y >= WINDOW_HEIGHT) mouse_y = (double) WINDOW_HEIGHT;
			if(mouse_x <= 0) mouse_x = (double) 0;
			if(mouse_y <= 0) mouse_y = (double) 0;

			trans_vec2[0] = 2.0f * mouse_x / (float) WINDOW_WIDTH -  1.0f;
			trans_vec2[1] = -2.0f * mouse_y / (float) WINDOW_HEIGHT + 1.0f;

			glm_mat4_copy(GLM_MAT4_IDENTITY, camera_mat);
			glm_translate(camera_mat, (vec3){trans_vec2[0], trans_vec2[1], 0.0f});
		}
	}

	/* Clean Up */
	basic_gl_release_configs();

	glDeleteProgram(shader_program_id); // Delete Shader Program

	tex2d_cleanup(&tex_mcface_id);      // Delete texture
	tex2d_cleanup(&tex_apple_id);
	tex2d_cleanup(&tex_eye_id);

	vertices_vao_cleanup(vert_arr_id); // Cleanup vertex buffer, array and elements buffer
	vertices_vbo_cleanup(vert_buff_id);
	vertices_ebo_cleanup(element_buff_id);

	glfwDestroyWindow(window);          // Destroy window object

	glfwTerminate(); // Terminate GLFW

	return 0;
}


static void basic_gl_init_configs(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


static void basic_gl_release_configs(void) {
	glDisable(GL_BLEND);
}