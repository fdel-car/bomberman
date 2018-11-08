/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameRenderer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 19:46:10 by jichen-m          #+#    #+#             */
/*   Updated: 2018/11/08 13:28:15 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "GameRenderer.hpp"

// === CONSTRUCTOR =============================================================

GameRenderer::GameRenderer(GameLogic *_mainGame)
{
	this->mainGame = _mainGame;
	// std::cout << "GLFW window" << std::endl;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw new std::runtime_error("NOP");
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	// glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	this->window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Bomberman", NULL, NULL); // size of screen will change
	if (!this->window)
	{
		glfwTerminate();
		std::cout << "Failed to create windows GLFW" << std::endl;
		throw new std::runtime_error("NOP");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width / 2) - (WINDOW_W / 2), (mode->height / 2) - (WINDOW_H / 2));
	glfwMakeContextCurrent(this->window);
	glfwGetWindowSize(window, &width, &height);
	glfwSetWindowUserPointer(window, this);
	// glViewport(0, 0, WINDOW_W, WINDOW_H);
	glfwSetKeyCallback(window, key_callback);
	glfwPollEvents();

	square_size = mainGame->get_square_size();
	x_offset = mainGame->get_x_offset();
	y_offset = mainGame->get_y_offset();

	//get top left of game screen
	start_x = -((WINDOW_W / 2.0f) - x_offset) / (WINDOW_W / 2.0f);
	start_y = ((WINDOW_H / 2.0f) - y_offset) / (WINDOW_H / 2.0f);

	//get the size of each square in the game screen
	square_percent_y = start_y / (_mainGame->get_map_h() / 2.0f);
	square_percent_x = (-start_x) / (_mainGame->get_map_w() / 2.0f);

	// Nuklear init
	ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end();

	return;
}

GameRenderer::GameRenderer(void)
{
	return;
}

GameRenderer::GameRenderer(GameRenderer const &src)
{
	*this = src;
	return;
}

GameRenderer::~GameRenderer(void)
{
	close_window();
	return;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

GameRenderer &GameRenderer::operator=(GameRenderer const &rhs)
{
	this->active = rhs.active;
	return *this;
}

// === ENDOPERATORS ============================================================

// === PRIVATE FUNCS ===========================================================

void GameRenderer::make_vao(GLuint &vbo)
{
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GameRenderer::drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint nbrOfSide, GLfloat **points)
{
	GLint nbrOfVertices = nbrOfSide + 2;

	GLfloat doublePi = 2.0f * 3.14159265358979f;

	GLfloat circleVerticesX[nbrOfVertices];
	GLfloat circleVerticesY[nbrOfVertices];
	GLfloat circleVerticesZ[nbrOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < nbrOfVertices; i++) {
		circleVerticesX[i] = x + (radius * cos(i * doublePi / nbrOfSide));
		circleVerticesY[i] = y + (radius * sin(i * doublePi / nbrOfSide));

		std::cout << circleVerticesX[i] << " " << circleVerticesY[i] << std::endl;
		circleVerticesZ[i] = z;
	}
	for (int i = 0; i < nbrOfVertices; i++) {
		(*points)[i * 3] = circleVerticesX[i];
		(*points)[(i * 3) + 1] = circleVerticesY[i];
		(*points)[(i * 3) + 2] = circleVerticesZ[i];
	}
}

void GameRenderer::init_buffer(int x, int y)
{
	float start_coor_X = start_x + (x * square_percent_x);
	float start_coor_Y = start_y - (y * square_percent_y);
	
	float xCenter = start_coor_X + (square_percent_x / 2);
	float yCenter = start_coor_Y - (square_percent_y / 2);
	int nbrOfSide = 120;
	float radiusX = square_percent_x / 2;
	float radiusY = square_percent_y / 2;

	GLint nbrOfVertices = nbrOfSide + 2;

	GLfloat doublePi = 2.0f * 3.14159265358979f;

	GLfloat circleVerticesX[nbrOfVertices];
	GLfloat circleVerticesY[nbrOfVertices];
	GLfloat circleVerticesZ[nbrOfVertices];

	circleVerticesX[0] = xCenter;
	circleVerticesY[0] = yCenter;
	circleVerticesZ[0] = 0;

	for (int i = 1; i < nbrOfVertices; i++) {
		circleVerticesX[i] = xCenter + (radiusX * cos(i * doublePi / nbrOfSide));
		circleVerticesY[i] = yCenter + (radiusY * sin(i * doublePi / nbrOfSide));
		circleVerticesZ[i] = 0;
	}
	GLfloat allCircleVertices[nbrOfVertices * 3];
	for (int i = 0; i < nbrOfVertices; i++) {
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}
	
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(allCircleVertices), allCircleVertices, GL_STATIC_DRAW);
	make_vao(vbo);

	init_shaders(GREEN_SHADER);
	init_program();
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, nbrOfVertices);
}

void GameRenderer::init_shaders(int type)
{
	//shader pour les vertex
	vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main() {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	if (type == WHITE_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);"
			"}";
	}
	else if (type == CYAN_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(0.0, 0.9, 0.7, 1.0);"
			"}";
	}
	else if (type == RED_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
			"}";
	}
	else if (type == GREEN_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(0.0, 0.9, 0.0, 1.0);"
			"}";
	}
	else if (type == YELLOW_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
			"}";
	}
	else if (type == GRAY_SHADER)
	{
		//shader pour dessiner ce qu'il y a entre les vertex
		fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main() {"
			"  frag_colour = vec4(185.0/255.0, 185.0/255.0, 146.0/255.0, 1.0);"
			"}";
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	//check if shaders compile fail
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		std::cout << "Vertex Shader compile failed" << std::endl;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
		std::cout << "Fragment Shader compile failed" << std::endl;
}

void GameRenderer::init_program(void)
{
	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
}

void GameRenderer::create_border(void)
{
	float epsilon_x = 1 / (WINDOW_W / 2.0f);
	float epsilon_y = 1 / (WINDOW_H / 2.0f);

	float vertex_border[] =
		{
			start_x, start_y + epsilon_y, 0.0f,				   // top-left
			-(start_x) + epsilon_x, start_y + epsilon_y, 0.0f, // top-right

			-(start_x) + epsilon_x, start_y + epsilon_y, 0.0f, // top-right
			-(start_x) + epsilon_x, -(start_y), 0.0f,		   // bottom-right

			-(start_x) + epsilon_x, -(start_y), 0.0f, // bottom-right
			start_x, -(start_y), 0.0f,				  // bottom-left

			start_x, -(start_y), 0.0f,		   // bottom-left
			start_x, start_y + epsilon_y, 0.0f // top-left
		};
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_border), vertex_border, GL_STATIC_DRAW);
	make_vao(vbo);

	init_shaders(1);
	init_program();
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	//drawing all the vertex of the triangle
	glDrawArrays(GL_LINE_LOOP, 0, 8);
}

void GameRenderer::create_grid(void)
{
	int linesCount = 78;
	int pointsCount = linesCount * 2;
	float lineSpacing = square_percent_x;
	float tmpX = start_x;
	float vertex_border[pointsCount * 3];
	for (int i = 0; i < pointsCount * 3; i++)
	{
		vertex_border[i] = 0;
	}

	// Vertical lines
	lineSpacing = square_percent_x;
	tmpX = start_x + lineSpacing;
	for (int i = 0; i < linesCount / 2; i++)
	{
		// Start point
		int lineIdx = i * 6;
		vertex_border[lineIdx] = tmpX;
		vertex_border[lineIdx + 1] = start_y;
		vertex_border[lineIdx + 2] = 0.0f;

		// End point
		vertex_border[lineIdx + 3] = tmpX;
		vertex_border[lineIdx + 4] = -(start_y);
		vertex_border[lineIdx + 5] = 0.0f;

		tmpX += lineSpacing;
	}
	// Horizontal lines
	lineSpacing = square_percent_y;
	tmpX = start_y - lineSpacing;
	for (int i = linesCount / 2; i < linesCount; i++)
	{
		// Start point
		int lineIdx = i * 6;
		vertex_border[lineIdx] = start_x;
		vertex_border[lineIdx + 1] = tmpX;
		vertex_border[lineIdx + 2] = 0.0f;

		// End point
		vertex_border[lineIdx + 3] = -start_x;
		vertex_border[lineIdx + 4] = tmpX;
		vertex_border[lineIdx + 5] = 0.0f;

		tmpX -= lineSpacing;
	}
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_border), vertex_border, GL_STATIC_DRAW);
	make_vao(vbo);

	init_shaders(1);
	init_program();
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	//drawing all the vertex of the triangle
	glDrawArrays(GL_LINES, 0, pointsCount);
}

void GameRenderer::draw_gui(void)
{
	nk_glfw3_new_frame();

	/* GUI */
	if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
					 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
	{
		enum
		{
			EASY,
			HARD
		};
		static int op = EASY;
		static int property = 20;
		nk_layout_row_static(ctx, 30, 80, 1);
		if (nk_button_label(ctx, "button"))
			fprintf(stdout, "button pressed\n");

		nk_layout_row_dynamic(ctx, 30, 2);
		if (nk_option_label(ctx, "easy", op == EASY))
			op = EASY;
		if (nk_option_label(ctx, "hard", op == HARD))
			op = HARD;

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(ctx, 20, 1);
		nk_label(ctx, "background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(ctx, 25, 1);
		if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400)))
		{
			nk_layout_row_dynamic(ctx, 120, 1);
			bg = nk_color_picker(ctx, bg, NK_RGBA);
			nk_layout_row_dynamic(ctx, 25, 1);
			bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
			bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
			bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
			bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
			nk_combo_end(ctx);
		}
	}
	nk_end(ctx);
	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void GameRenderer::draw_player(std::tuple<int, int> &player_pos)
{
	init_buffer(std::get<0>(player_pos), std::get<1>(player_pos));
	// init_shaders(GREEN_SHADER);
	// init_program();
	// glUseProgram(shader_program);
	// glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, 3);
}
// === END PRIVATE FUNCS =======================================================

// === OVERRIDES ===============================================================
void GameRenderer::get_user_input(void)
{
	glfwPollEvents();
}

void GameRenderer::refresh_window(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	create_border();
	draw_player(mainGame->get_player_pos());

	create_grid();

	draw_gui();

	//put everything to screen
	glfwSwapBuffers(this->window);
}

void GameRenderer::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_RIGHT:
			mainGame->button_pressed(KEY_RIGHT);
			break;
		case GLFW_KEY_LEFT:
			mainGame->button_pressed(KEY_LEFT);
			break;
		case GLFW_KEY_UP:
			mainGame->button_pressed(KEY_UP);
			break;
		case GLFW_KEY_DOWN:
			mainGame->button_pressed(KEY_DOWN);
			break;
		default:
			mainGame->button_pressed(glfwGetKeyName(key, scancode));
			break;
		}
	}
	(void)key;
	(void)scancode;
	(void)action;
	(void)window;
	(void)mods;
}

void GameRenderer::close_window()
{
	// std::cout << "Destroing Glfw window" << std::endl;
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
}

void GameRenderer::error_callback(int error, const char *description)
{
	std::cerr << "Error n." << error << ": " << description << std::endl;
}
// === END OVERRIDES ===========================================================

GameLogic *GameRenderer::mainGame = NULL;
