#pragma once

#define NK_SHADER_VERSION "#version 410\n"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define NK_GLFW_TEXT_MAX 256
#define NK_GLFW_DOUBLE_CLICK_LO 0.02
#define NK_GLFW_DOUBLE_CLICK_HI 0.2

// #include <GLFW/glfw3.h>
#include "engine/Engine.hpp"
#include "nuklear/nuklear.h"

enum theme { THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK };

struct media {
	std::map<std::string, struct nk_font *> myFonts;
	std::map<std::string, struct nk_image> myImages;
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
	struct nk_font_atlas *atlas;
	struct nk_vec2 fb_scale;
	unsigned int text[NK_GLFW_TEXT_MAX];
	int text_len;
	struct nk_vec2 scroll;
	double last_button_click;
	int is_double_click_down;
	struct nk_vec2 double_click_pos;
};

class GameRenderer;

class GUI {
   public:
	GUI(GameRenderer *gameRenderer, GLFWwindow *window,
		std::vector<std::tuple<float, std::string, std::string>> vFontPath =
			std::vector<std::tuple<float, std::string, std::string>>(),
		std::vector<std::tuple<std::string, std::string>> vImagePath =
			std::vector<std::tuple<std::string, std::string>>());
	~GUI();

	static struct nk glfw;

	/*
	** Rendering
	*/

	void nkNewFrame();
	void nkRender();

	/*
	** Style
	*/

	void getDefaultStyle(enum theme theme, std::map<int, nk_color> *styleMap);
	void setStyle(std::map<int, nk_color> &styleMap);
	void setAssetImages(
		std::vector<std::tuple<std::string, std::string>> vFontImage);

	bool uiStartBlock(const char *id, const char *title, struct nk_rect bounds,
					  nk_flags flags);
	void uiEndBlock();
	void uiApplyFont(std::string fontName);
	void uiApplyDefaultFont();
	void uiSetDefaultFont(std::string fontName);
	void uiSetImage(std::string imgName);
	void uiHeader(const char *title, nk_flags flags, int rowSize,
				  std::string fontName = "");
	void uiText(std::string text, nk_flags textFlags,
				std::string fontName = "");
	void uiWidget(float height, std::string fontName = "");
	bool uiButton(int width, int height, nk_flags flags, std::string text = "",
				  std::string imgName = "", std::string fontName = "",
				  bool isAlone = true);
	void uiWidgetCentered(float height, std::string fontName = "");
	void uiDialogBox(const char *name, std::string imgName, const char *text,
					 bool isImgLeft, size_t maxCharPerLine, int nbrOfLine,
					 nk_flags textPosition = NK_TEXT_LEFT,
					 std::string fontText = "", std::string fontTitle = "");
	bool uiHorizontalSelection(int widgetWidth, std::string leftText,
							   std::string rightText, int *choice, int maxSize, int height);
	bool uiHover();
	void uiEditString(nk_flags flags, char *fieldBuffer, int *len, int max,
					  nk_plugin_filter filter);
	void uiHorizontalEditString(int widgetWidth, std::string leftText,
								nk_flags flags, char *fieldBuffer, int *len,
								int max, nk_plugin_filter filter, int height);
	void uiRowMultipleElem(bool isStart, int height = 30, int nbrOfElem = 1,
						   nk_layout_format flags = NK_STATIC);
	void uiAddElemInRow(int width);
	void uiAddElemOffset(int offset);

   private:
	GameRenderer *_gameRenderer;

	GUI();
	GUI(GUI const &src);
	GUI &operator=(GUI const &rhs);

	/*
	** Contain all fonts && images/icons
	*/

	struct media *_media;

	/*
	** Init GUI (nuklear)
	*/

	void _nkInit(GLFWwindow *win);
	void _nkDeviceCreate();

	static void nkClipboardCopy(nk_handle usr, const char *text, int len);
	static void nkClipboardPaste(nk_handle usr, struct nk_text_edit *edit);
	static void nkCharCallback(GLFWwindow *win, unsigned int codepoint);
	static void nkScrollCallback(GLFWwindow *win, double xoff, double yoff);
	static void nkMouseButtonCallback(GLFWwindow *win, int button, int action,
									  int mods);

	/*
	** Init && Set : font && image
	*/

	void _nkFontStashBegin();
	void _nkFontStashEnd();
	void _deviceUploadAtlas(const void *image, int width, int height);
	void _setFonts(
		std::vector<std::tuple<float, std::string, std::string>> &vFontPath);
	void _setImages(
		std::vector<std::tuple<std::string, std::string>> &vImagePath);
	struct nk_image iconLoad(const char *filename);

	/*
	** Free GUI (nuklear)
	*/

	void _nkDeviceDestroy();
	void _nkShutdown();
};
