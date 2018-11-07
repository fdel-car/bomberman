/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GlfwGUI.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 19:46:10 by jichen-m          #+#    #+#             */
/*   Updated: 2018/09/24 18:12:06 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GlfwGUI.hpp"

// === CONSTRUCTOR =============================================================

GlfwGUI::GlfwGUI(MainGame *_mainGame)
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	this->window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Nibbler GLFW", NULL, NULL); // size of screen will change
	if (!this->window)
	{
		glfwTerminate();
		std::cout << "Failed to create windows GLFW" << std::endl;
		throw new std::runtime_error("NOP");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width / 2) - (WINDOW_W / 2), (mode->height / 2) - (WINDOW_H / 2));
	glfwMakeContextCurrent(this->window);
	glfwSetWindowUserPointer(window, this);
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

	this->counter = 0.0f;
	return;
}
GlfwGUI::GlfwGUI(void)
{
	return;
}

GlfwGUI::GlfwGUI(GlfwGUI const &src)
{
	*this = src;
	return;
}

GlfwGUI::~GlfwGUI(void)
{
	close_window();
	return;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

GlfwGUI &GlfwGUI::operator=(GlfwGUI const &rhs)
{
	this->active = rhs.active;
	return *this;
}

// === ENDOPERATORS ============================================================

// === PRIVATE FUNCS ===========================================================

void GlfwGUI::make_vao(GLuint &vbo)
{
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void GlfwGUI::init_buffer(int x, int y)
{
	float start_coor_X = start_x + (x * square_percent_x);
	float start_coor_Y = start_y - (y * square_percent_y);

	//POINTS
	float points[] = {
		start_coor_X, start_coor_Y, 0.0f,
		start_coor_X, start_coor_Y - square_percent_y, 0.0f,
		start_coor_X + square_percent_x, start_coor_Y, 0.0f,

		start_coor_X + square_percent_x, start_coor_Y, 0.0f,
		start_coor_X + square_percent_x, start_coor_Y - square_percent_y, 0.0f,
		start_coor_X, start_coor_Y - square_percent_y, 0.0f};
	//BUFFER
	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	make_vao(vbo);
}

void GlfwGUI::init_shaders(int type)
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

void GlfwGUI::init_program(void)
{
	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
}

void GlfwGUI::create_border(void)
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

void GlfwGUI::create_grid(void)
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
	// for (int i = 0; i <
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
// === END PRIVATE FUNCS =======================================================

// === OVERRIDES ===============================================================
void GlfwGUI::get_user_input(void)
{
	glfwPollEvents();
}

void GlfwGUI::refresh_window(void)
{
	//only for test to see if each frame change color
	// this->counter = this->counter + 0.2f;
	if (this->counter == 1.0f)
		this->counter = 0.0f;
	glClearColor(this->counter, this->counter, this->counter, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	create_border();

	// Add snakes
	std::tuple<int, int> &body_part = mainGame->get_player_pos(); // access by reference to avoid copying
	init_buffer(std::get<0>(body_part), std::get<1>(body_part));
	init_shaders(GREEN_SHADER);
	init_program();
	glUseProgram(shader_program);
	glBindVertexArray(vao);
	//drawing all the vertex of the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	create_grid();

	//put everything to screen
	glfwSwapBuffers(this->window);
}

void GlfwGUI::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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

void GlfwGUI::close_window()
{
	// std::cout << "Destroing Glfw window" << std::endl;
	if (window)
		glfwDestroyWindow(this->window);
	glfwTerminate();
}

void GlfwGUI::error_callback(int error, const char *description)
{
	std::cerr << "Error n." << error << ": " << description << std::endl;
}
// === END OVERRIDES ===========================================================

MainGame *GlfwGUI::mainGame = NULL;
