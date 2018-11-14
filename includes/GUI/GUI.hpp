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

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK};

struct media {
    std::map<std::string, struct nk_font * > myFonts;
	std::map<std::string, struct nk_image > myImages;
	std::string defaultFont;
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

struct nk {
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
};

class GUI {

public:

	GUI();
	GUI(GLFWwindow *window,
		std::vector<std::tuple<float,const char *,std::string>> vFontPath = std::vector<std::tuple<float,const char *,std::string>>(),
		std::vector<std::tuple<const char *,std::string>> vImagePath = std::vector<std::tuple<const char *,std::string>>());
	~GUI();

	GUI &operator=(GUI const &rhs);

	void drawGUI();

	static struct nk glfw;

	/*
	** Rendering
	*/

	void nkNewFrame();
	void nkRender();

	/*
	** Style
	*/

	void setStyle(enum theme theme);

	bool uiStartBlock(const char * id, const char * title, struct nk_rect bounds, nk_flags flags);
	void uiEndBlock();
	void uiApplyFont(std::string fontName);
	void uiApplyDefaultFont();
	void uiSetDefaultFont(std::string fontName);
	void uiSetImage(std::string imgName);
	void uiHeader(const char * title, nk_flags flags, std::string fontName = "");
	void uiWidget(float height, std::string fontName = "");
	void uiWidgetCentered(float height, std::string fontName = "");
	void uiDialogBox(const char * name, std::string imgName, const char * text, bool isImgLeft, size_t maxCharPerLine, int nbrOfLine, nk_flags textPosition = NK_TEXT_LEFT, std::string fontText = "", std::string fontTitle = "");

private:
	/*
	** Contain all fonts && images/icons
	*/

	struct media * _media;

	/*
	** Init GUI (nuklear)
	*/

	void _nkInit(GLFWwindow *win);
	void _nkDeviceCreate();

	static void nkClipboardCopy(nk_handle usr, const char *text, int len);
	static void nkClipboardPaste(nk_handle usr, struct nk_text_edit *edit);
	static void nkCharCallback(GLFWwindow *win, unsigned int codepoint);
	static void nkScrollCallback(GLFWwindow *win, double xoff, double yoff);
	static void nkMouseButtonCallback(GLFWwindow *win, int button, int action, int mods);

	/*
	** Init && Set : font && image
	*/

	void _nkFontStashBegin();
	void _nkFontStashEnd();
	void _deviceUploadAtlas(const void *image, int width, int height);
	void _setFonts(std::vector< std::tuple< float, const char *, std::string > > &vFontPath);
	void _setImages(std::vector< std::tuple< const char *, std::string > > &vImagePath);
	struct nk_image iconLoad(const char *filename);

	/*
	** Free GUI (nuklear)
	*/

	void _nkDeviceDestroy();
	void _nkShutdown();

};
