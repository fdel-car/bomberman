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
#include "GameLogic.hpp"

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw new std::runtime_error("NOP");
	}

	glfwSetKeyCallback(window, key_callback);
	glfwPollEvents();

	square_size = mainGame->getSquareSize();
	x_offset = mainGame->getXOffset();
	y_offset = mainGame->getYOffset();

	//get top left of game screen
	start_x = -((WINDOW_W / 2.0f) - x_offset) / (WINDOW_W / 2.0f);
	start_y = ((WINDOW_H / 2.0f) - y_offset) / (WINDOW_H / 2.0f);

	//get the size of each square in the game screen
	square_percent_y = start_y / (_mainGame->getMapH() / 2.0f);
	square_percent_x = (-start_x) / (_mainGame->getMapW() / 2.0f);

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
	closeWindow();
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
	float start_coor_X = start_x + (std::get<0>(player_pos) * square_percent_x);
	float start_coor_Y = start_y - (std::get<1>(player_pos) * square_percent_y);

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

	for (int i = 1; i < nbrOfVertices; i++)
	{
		circleVerticesX[i] = xCenter + (radiusX * cos(i * doublePi / nbrOfSide));
		circleVerticesY[i] = yCenter + (radiusY * sin(i * doublePi / nbrOfSide));
		circleVerticesZ[i] = 0;
	}
	GLfloat allCircleVertices[nbrOfVertices * 3];
	for (int i = 0; i < nbrOfVertices; i++)
	{
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
// === END PRIVATE FUNCS =======================================================

// === OVERRIDES ===============================================================
void GameRenderer::getUserInput(void)
{
	glfwPollEvents();
}

void GameRenderer::refreshWindow(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	create_border();
	draw_player(mainGame->getPlayerPos());

	create_grid();

	draw_gui();

	//put everything to screen
	glfwSwapBuffers(this->window);
}

void GameRenderer::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_UNKNOWN:
			mainGame->buttonStateChanged("UNKNOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SPACE:
			mainGame->buttonStateChanged("SPACE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_APOSTROPHE:
			mainGame->buttonStateChanged("APOSTROPHE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_COMMA:
			mainGame->buttonStateChanged("COMMA", action == GLFW_PRESS);
			break;

		case GLFW_KEY_MINUS:
			mainGame->buttonStateChanged("MINUS", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PERIOD:
			mainGame->buttonStateChanged("PERIOD", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SLASH:
			mainGame->buttonStateChanged("SLASH", action == GLFW_PRESS);
			break;

		case GLFW_KEY_0:
			mainGame->buttonStateChanged("0", action == GLFW_PRESS);
			break;

		case GLFW_KEY_1:
			mainGame->buttonStateChanged("1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_2:
			mainGame->buttonStateChanged("2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_3:
			mainGame->buttonStateChanged("3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_4:
			mainGame->buttonStateChanged("4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_5:
			mainGame->buttonStateChanged("5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_6:
			mainGame->buttonStateChanged("6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_7:
			mainGame->buttonStateChanged("7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_8:
			mainGame->buttonStateChanged("8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_9:
			mainGame->buttonStateChanged("9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SEMICOLON:
			mainGame->buttonStateChanged(";", action == GLFW_PRESS);
			break;

		case GLFW_KEY_EQUAL:
			mainGame->buttonStateChanged("EQUAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_A:
			mainGame->buttonStateChanged("A", action == GLFW_PRESS);
			break;

		case GLFW_KEY_B:
			mainGame->buttonStateChanged("B", action == GLFW_PRESS);
			break;

		case GLFW_KEY_C:
			mainGame->buttonStateChanged("C", action == GLFW_PRESS);
			break;

		case GLFW_KEY_D:
			mainGame->buttonStateChanged("D", action == GLFW_PRESS);
			break;

		case GLFW_KEY_E:
			mainGame->buttonStateChanged("E", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F:
			mainGame->buttonStateChanged("F", action == GLFW_PRESS);
			break;

		case GLFW_KEY_G:
			mainGame->buttonStateChanged("G", action == GLFW_PRESS);
			break;

		case GLFW_KEY_H:
			mainGame->buttonStateChanged("H", action == GLFW_PRESS);
			break;

		case GLFW_KEY_I:
			mainGame->buttonStateChanged("I", action == GLFW_PRESS);
			break;

		case GLFW_KEY_J:
			mainGame->buttonStateChanged("L", action == GLFW_PRESS);
			break;

		case GLFW_KEY_K:
			mainGame->buttonStateChanged("K", action == GLFW_PRESS);
			break;

		case GLFW_KEY_L:
			mainGame->buttonStateChanged("L", action == GLFW_PRESS);
			break;

		case GLFW_KEY_M:
			mainGame->buttonStateChanged("M", action == GLFW_PRESS);
			break;

		case GLFW_KEY_N:
			mainGame->buttonStateChanged("N", action == GLFW_PRESS);
			break;

		case GLFW_KEY_O:
			mainGame->buttonStateChanged("O", action == GLFW_PRESS);
			break;

		case GLFW_KEY_P:
			mainGame->buttonStateChanged("P", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Q:
			mainGame->buttonStateChanged("Q", action == GLFW_PRESS);
			break;

		case GLFW_KEY_R:
			mainGame->buttonStateChanged("R", action == GLFW_PRESS);
			break;

		case GLFW_KEY_S:
			mainGame->buttonStateChanged("S", action == GLFW_PRESS);
			break;

		case GLFW_KEY_T:
			mainGame->buttonStateChanged("T", action == GLFW_PRESS);
			break;

		case GLFW_KEY_U:
			mainGame->buttonStateChanged("U", action == GLFW_PRESS);
			break;

		case GLFW_KEY_V:
			mainGame->buttonStateChanged("V", action == GLFW_PRESS);
			break;

		case GLFW_KEY_W:
			mainGame->buttonStateChanged("W", action == GLFW_PRESS);
			break;

		case GLFW_KEY_X:
			mainGame->buttonStateChanged("X", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Y:
			mainGame->buttonStateChanged("Y", action == GLFW_PRESS);
			break;

		case GLFW_KEY_Z:
			mainGame->buttonStateChanged("Z", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_BRACKET:
			mainGame->buttonStateChanged("[", action == GLFW_PRESS);
			break;

		case GLFW_KEY_BACKSLASH:
			mainGame->buttonStateChanged("\\", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_BRACKET:
			mainGame->buttonStateChanged("]", action == GLFW_PRESS);
			break;

		case GLFW_KEY_GRAVE_ACCENT:
			mainGame->buttonStateChanged("`", action == GLFW_PRESS);
			break;

		case GLFW_KEY_WORLD_1:
			mainGame->buttonStateChanged("WORLD_1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_WORLD_2:
			mainGame->buttonStateChanged("WORLD_2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_ESCAPE:
			mainGame->buttonStateChanged("ESCAPE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_ENTER:
			mainGame->buttonStateChanged("ENTER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_TAB:
			mainGame->buttonStateChanged("TAB", action == GLFW_PRESS);
			break;

		case GLFW_KEY_BACKSPACE:
			mainGame->buttonStateChanged("BACKSPACE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_INSERT:
			mainGame->buttonStateChanged("INSERT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_DELETE:
			mainGame->buttonStateChanged("DELETE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT:
			mainGame->buttonStateChanged("RIGHT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT:
			mainGame->buttonStateChanged("LEFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_DOWN:
			mainGame->buttonStateChanged("DOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_UP:
			mainGame->buttonStateChanged("UP", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAGE_UP:
			mainGame->buttonStateChanged("PAGE_UP", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAGE_DOWN:
			mainGame->buttonStateChanged("PAGE_DOWN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_HOME:
			mainGame->buttonStateChanged("HOME", action == GLFW_PRESS);
			break;

		case GLFW_KEY_END:
			mainGame->buttonStateChanged("END", action == GLFW_PRESS);
			break;

		case GLFW_KEY_CAPS_LOCK:
			mainGame->buttonStateChanged("CAPS_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_SCROLL_LOCK:
			mainGame->buttonStateChanged("SCROLL_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_NUM_LOCK:
			mainGame->buttonStateChanged("NUM_LOCK", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PRINT_SCREEN:
			mainGame->buttonStateChanged("PRINT_SCREEN", action == GLFW_PRESS);
			break;

		case GLFW_KEY_PAUSE:
			mainGame->buttonStateChanged("PAUSE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F1:
			mainGame->buttonStateChanged("F1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F2:
			mainGame->buttonStateChanged("F2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F3:
			mainGame->buttonStateChanged("F3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F4:
			mainGame->buttonStateChanged("F4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F5:
			mainGame->buttonStateChanged("F5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F6:
			mainGame->buttonStateChanged("F6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F7:
			mainGame->buttonStateChanged("F7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F8:
			mainGame->buttonStateChanged("F8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F9:
			mainGame->buttonStateChanged("F9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F10:
			mainGame->buttonStateChanged("F10", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F11:
			mainGame->buttonStateChanged("F11", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F12:
			mainGame->buttonStateChanged("F12", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F13:
			mainGame->buttonStateChanged("F13", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F14:
			mainGame->buttonStateChanged("F14", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F15:
			mainGame->buttonStateChanged("F15", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F16:
			mainGame->buttonStateChanged("F16", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F17:
			mainGame->buttonStateChanged("F17", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F18:
			mainGame->buttonStateChanged("F18", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F19:
			mainGame->buttonStateChanged("F19", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F20:
			mainGame->buttonStateChanged("F20", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F21:
			mainGame->buttonStateChanged("F21", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F22:
			mainGame->buttonStateChanged("F22", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F23:
			mainGame->buttonStateChanged("F23", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F24:
			mainGame->buttonStateChanged("F24", action == GLFW_PRESS);
			break;

		case GLFW_KEY_F25:
			mainGame->buttonStateChanged("F25", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_0:
			mainGame->buttonStateChanged("KP_0", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_1:
			mainGame->buttonStateChanged("KP_1", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_2:
			mainGame->buttonStateChanged("KP_2", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_3:
			mainGame->buttonStateChanged("KP_3", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_4:
			mainGame->buttonStateChanged("KP_4", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_5:
			mainGame->buttonStateChanged("KP_5", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_6:
			mainGame->buttonStateChanged("KP_6", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_7:
			mainGame->buttonStateChanged("KP_7", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_8:
			mainGame->buttonStateChanged("KP_8", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_9:
			mainGame->buttonStateChanged("KP_9", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_DECIMAL:
			mainGame->buttonStateChanged("KP_DECIMAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_DIVIDE:
			mainGame->buttonStateChanged("KP_DIVIDE", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_MULTIPLY:
			mainGame->buttonStateChanged("KP_MULTIPLY", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_SUBTRACT:
			mainGame->buttonStateChanged("KP_SUBTRACT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_ADD:
			mainGame->buttonStateChanged("KP_ADD", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_ENTER:
			mainGame->buttonStateChanged("KP_ENTER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_KP_EQUAL:
			mainGame->buttonStateChanged("KP_EQUAL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_SHIFT:
			mainGame->buttonStateChanged("LEFT_SHIFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_CONTROL:
			mainGame->buttonStateChanged("LEFT_CONTROL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_ALT:
			mainGame->buttonStateChanged("LEFT_ALT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_LEFT_SUPER:
			mainGame->buttonStateChanged("LEFT_SUPER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_SHIFT:
			mainGame->buttonStateChanged("RIGHT_SHIFT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_CONTROL:
			mainGame->buttonStateChanged("RIGHT_CONTROL", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_ALT:
			mainGame->buttonStateChanged("RIGHT_ALT", action == GLFW_PRESS);
			break;

		case GLFW_KEY_RIGHT_SUPER:
			mainGame->buttonStateChanged("RIGHT_SUPER", action == GLFW_PRESS);
			break;

		case GLFW_KEY_MENU:
			mainGame->buttonStateChanged("MENU", action == GLFW_PRESS);
			break;
		default:
			break;
		}
	}
	(void)key;
	(void)scancode;
	(void)action;
	(void)window;
	(void)mods;
}

void GameRenderer::closeWindow()
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
