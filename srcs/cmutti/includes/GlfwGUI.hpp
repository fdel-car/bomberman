// #pragma once

#ifndef GLFWGUI_HPP
#define GLFWGUI_HPP

#define GLFW_INCLUDE_GLCOREARB

#include <iostream>
#include "GLFW/glfw3.h"
#include "MainGame.hpp"

// Nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "Nuklear/nuklear.h"
#include "Nuklear/nuklear_glfw_gl3.h"

// Custom Defines
#define WHITE_SHADER 1
#define GREEN_SHADER 2
#define RED_SHADER 3
#define CYAN_SHADER 4
#define YELLOW_SHADER 5
#define GRAY_SHADER 6

class MainGame;

class GlfwGUI
{
  public:
	static MainGame *mainGame;

	GlfwGUI(MainGame *mainGame);
	~GlfwGUI(void);

	void get_user_input(void);
	void refresh_window(void);
	void close_window(void);

	bool active;

  private:
	GlfwGUI(void);
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void error_callback(int error, const char *description);

	GlfwGUI(GlfwGUI const &src);

	GlfwGUI &operator=(GlfwGUI const &rhs);

	void init_buffer(int x, int y);
	void init_shaders(int type);
	void init_program(void);
	void create_border(void);
	void create_grid(void);
	void draw_gui(void);
	void draw_player(std::tuple<int, int> &player_pos);
	void make_vao(GLuint &vbo);

	// General vars
	GLFWwindow *window;
	int width = 0;
	int height = 0;
	int x_offset;
	int y_offset;
	int square_size;
	float start_x;
	float start_y;
	float square_percent_y;
	float square_percent_x;

	// Nuklear vars
	struct nk_context *ctx;
	struct nk_colorf bg;

	// Rendering vars
	GLuint vbo;
	GLuint vao;
	const char *vertex_shader;
	const char *fragment_shader;
	GLuint vs;
	GLuint fs;
	GLuint shader_program;
};

#endif