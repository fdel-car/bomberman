#ifdef __APPLE__
  #define NK_SHADER_VERSION "#version 150\n"
#else
  #define NK_SHADER_VERSION "#version 300 es\n"
#endif

#pragma once

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include "header.hpp"
#include <GLFW/glfw3.h>
#include "nuklear/nuklear.h"
#include "stb/stb_image.h"

struct media {
    struct nk_font *font_14;
    struct nk_font *font_18;
    struct nk_font *font_20;
    struct nk_font *font_22;

    struct nk_image unchecked;
    struct nk_image checked;
    struct nk_image rocket;
    struct nk_image cloud;
    struct nk_image pen;
    struct nk_image play;
    struct nk_image pause;
    struct nk_image stop;
    struct nk_image prev;
    struct nk_image next;
    struct nk_image tools;
    struct nk_image dir;
    struct nk_image copy;
    struct nk_image convert;
    struct nk_image del;
    struct nk_image edit;
    struct nk_image images[9];
    struct nk_image menu[6];
};

struct nk_glfw_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

struct device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, vao, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};

class GUI {

public:
	GUI();
	GUI(GLFWwindow *window);
	~GUI();

	GUI &operator=(GUI const &rhs);

	void drawGUI();

private:
	void _deviceInit(struct device *dev);

	GLFWwindow * _win;
	// struct device _device;
    // // struct nk_font_atlas _atlas;
    // struct media _media;
    // struct nk_context _ctx;
	// struct nk_font_atlas *_atlas;

	/*
	** V1 start
	*/

	// void set_style(struct nk_context *ctx);

	// Nuklear vars
	// struct nk_context *ctx;
	// struct nk_colorf bg;
	// struct nk_font_atlas *atlas;

	/*
	** V1 end
	*/

};
