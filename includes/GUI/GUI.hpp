#ifdef __APPLE__
  #define NK_SHADER_VERSION "#version 150\n"
#else
  #define NK_SHADER_VERSION "#version 300 es\n"
#endif

#pragma once

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define NK_GLFW_TEXT_MAX 256
#define NK_GLFW_DOUBLE_CLICK_LO 0.02
#define NK_GLFW_DOUBLE_CLICK_HI 0.2

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

struct nk_vertex {
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

static struct nk {
    GLFWwindow *win;
    int width, height;
    int display_width, display_height;
    struct device ogl;
    struct nk_context ctx;
    struct nk_font_atlas * atlas;
    struct nk_vec2 fb_scale;
    unsigned int text[NK_GLFW_TEXT_MAX];
    int text_len;
    struct nk_vec2 scroll;
    double last_button_click;
    int is_double_click_down;
    struct nk_vec2 double_click_pos;
} glfw;

class GUI {

public:
	GUI();
	GUI(GLFWwindow *window);
	~GUI();

	GUI &operator=(GUI const &rhs);

	void drawGUI();

private:
	// struct nk glfw;
	enum nk_glfw_init_state
	{
	    NK_GLFW3_DEFAULT = 0,
	    NK_GLFW3_INSTALL_CALLBACKS
	};

	struct nk_context * nkInit(GLFWwindow *win, enum nk_glfw_init_state);
	static void nkClipboardCopy(nk_handle usr, const char *text, int len);
	static void nkClipboardPaste(nk_handle usr, struct nk_text_edit *edit);
	void nkDeviceCreate();
	static void nkCharCallback(GLFWwindow *win, unsigned int codepoint);
	static void nkScrollCallback(GLFWwindow *win, double xoff, double yoff);
	static void nkMouseButtonCallback(GLFWwindow *win, int button, int action, int mods);

	void nkNewFrame();

	void deviceUploadAtlas(const void *image, int width, int height);
	void nkRender(enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer);
	// void nk_glfw3_shutdown(void);
	void nkFontStashBegin(/*struct nk_font_atlas **atlas*/);
	void nkFontStashEnd();
	//
	// void nk_glfw3_device_destroy(void);
	//

    struct media * _media;

};
