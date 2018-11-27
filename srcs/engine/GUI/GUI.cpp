#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

#include "engine/GUI/GUI.hpp"

struct nk GUI::glfw = nk();

GUI::GUI(GLFWwindow *window,
		 std::vector<std::tuple<float, std::string, std::string>> vFontPath,
		 std::vector<std::tuple<std::string, std::string>> vImagePath)
	: _media(new media()) {
	GUI::glfw.atlas = new nk_font_atlas();
	_nkInit(window);
	_nkFontStashBegin();
	if (!vFontPath.empty()) _setFonts(vFontPath);
	_nkFontStashEnd();
	if (!vImagePath.empty()) _setImages(vImagePath);
}

GUI::~GUI() {
	_nkShutdown();
	delete _media;
}

// Prepare window for build a GUI
void GUI::nkNewFrame() {
	int i;
	double x, y;
	struct nk_context *ctx = &GUI::glfw.ctx;
	struct GLFWwindow *win = GUI::glfw.win;

	glfwGetWindowSize(win, &GUI::glfw.width, &GUI::glfw.height);
	glfwGetFramebufferSize(win, &GUI::glfw.display_width,
						   &GUI::glfw.display_height);
	GUI::glfw.fb_scale.x =
		(float)GUI::glfw.display_width / (float)GUI::glfw.width;
	GUI::glfw.fb_scale.y =
		(float)GUI::glfw.display_height / (float)GUI::glfw.height;

	nk_input_begin(ctx);
	for (i = 0; i < GUI::glfw.text_len; ++i)
		nk_input_unicode(ctx, GUI::glfw.text[i]);

#ifdef NK_GLFW_GL3_MOUSE_GRABBING
	// Optional grabbing behavior
	if (ctx->input.mouse.grab)
		glfwSetInputMode(GUI::glfw.win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else if (ctx->input.mouse.ungrab)
		glfwSetInputMode(GUI::glfw.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif

	nk_input_key(ctx, NK_KEY_DEL,
				 glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_ENTER,
				 glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_BACKSPACE,
				 glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_DOWN,
				 glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TEXT_START,
				 glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TEXT_END,
				 glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_START,
				 glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_END,
				 glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_DOWN,
				 glfwGetKey(win, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_UP,
				 glfwGetKey(win, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SHIFT,
				 glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
					 glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
		glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
		nk_input_key(ctx, NK_KEY_COPY,
					 glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_PASTE,
					 glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_CUT,
					 glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_UNDO,
					 glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_REDO,
					 glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT,
					 glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT,
					 glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_LINE_START,
					 glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_LINE_END,
					 glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
	} else {
		nk_input_key(ctx, NK_KEY_LEFT,
					 glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_RIGHT,
					 glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_COPY, 0);
		nk_input_key(ctx, NK_KEY_PASTE, 0);
		nk_input_key(ctx, NK_KEY_CUT, 0);
		nk_input_key(ctx, NK_KEY_SHIFT, 0);
	}
	glfwGetCursorPos(win, &x, &y);
	nk_input_motion(ctx, (int)x, (int)y);

#ifdef NK_GLFW_GL3_MOUSE_GRABBING
	if (ctx->input.mouse.grabbed) {
		glfwSetCursorPos(GUI::glfw.win, ctx->input.mouse.prev.x,
						 ctx->input.mouse.prev.y);
		ctx->input.mouse.pos.x = ctx->input.mouse.prev.x;
		ctx->input.mouse.pos.y = ctx->input.mouse.prev.y;
	}
#endif

	nk_input_button(
		ctx, NK_BUTTON_LEFT, (int)x, (int)y,
		glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	nk_input_button(
		ctx, NK_BUTTON_MIDDLE, (int)x, (int)y,
		glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
	nk_input_button(
		ctx, NK_BUTTON_RIGHT, (int)x, (int)y,
		glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	nk_input_button(ctx, NK_BUTTON_DOUBLE, (int)GUI::glfw.double_click_pos.x,
					(int)GUI::glfw.double_click_pos.y,
					GUI::glfw.is_double_click_down);
	nk_input_scroll(ctx, GUI::glfw.scroll);
	nk_input_end(&GUI::glfw.ctx);
	GUI::glfw.text_len = 0;
	GUI::glfw.scroll = nk_vec2(0, 0);
}

// Prepare window display the GUI
void GUI::nkRender() {
	struct device *dev = &GUI::glfw.ogl;
	struct nk_buffer vbuf, ebuf;
	GLfloat ortho[4][4] = {
		{2.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, -2.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, -1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f, 1.0f},
	};
	ortho[0][0] /= (GLfloat)GUI::glfw.width;
	ortho[1][1] /= (GLfloat)GUI::glfw.height;

	// Setup global state
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup program
	glUseProgram(dev->prog);
	glUniform1i(dev->uniform_tex, 0);
	glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
	glViewport(0, 0, (GLsizei)GUI::glfw.display_width,
			   (GLsizei)GUI::glfw.display_height);
	{
		// Convert from command queue into draw list and draw to screen
		const struct nk_draw_command *cmd;
		void *vertices, *elements;
		const nk_draw_index *offset = NULL;

		// Allocate vertex and element buffer
		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER, NULL, GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_VERTEX_BUFFER, NULL,
					 GL_STREAM_DRAW);

		// Load draw vertices & elements directly into vertex + element buffer
		vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		{
			// Fill convert configuration
			struct nk_convert_config config;
			static const struct nk_draw_vertex_layout_element vertex_layout[] =
				{{NK_VERTEX_POSITION, NK_FORMAT_FLOAT,
				  NK_OFFSETOF(struct nk_vertex, position)},
				 {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT,
				  NK_OFFSETOF(struct nk_vertex, uv)},
				 {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8,
				  NK_OFFSETOF(struct nk_vertex, col)},
				 {NK_VERTEX_LAYOUT_END}};
			NK_MEMSET(&config, 0, sizeof(config));
			config.vertex_layout = vertex_layout;
			config.vertex_size = sizeof(struct nk_vertex);
			config.vertex_alignment = NK_ALIGNOF(struct nk_vertex);
			config.null = dev->null;
			config.circle_segment_count = 22;
			config.curve_segment_count = 22;
			config.arc_segment_count = 22;
			config.global_alpha = 1.0f;
			config.shape_AA = NK_ANTI_ALIASING_ON;
			config.line_AA = NK_ANTI_ALIASING_ON;

			// Setup buffers to load vertices and elements
			nk_buffer_init_fixed(&vbuf, vertices, (size_t)MAX_VERTEX_BUFFER);
			nk_buffer_init_fixed(&ebuf, elements, (size_t)MAX_VERTEX_BUFFER);
			nk_convert(&GUI::glfw.ctx, &dev->cmds, &vbuf, &ebuf, &config);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		// Iterate over and execute each draw command
		nk_draw_foreach(cmd, &GUI::glfw.ctx, &dev->cmds) {
			if (!cmd->elem_count) continue;
			glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
			glScissor((GLint)(cmd->clip_rect.x * GUI::glfw.fb_scale.x),
					  (GLint)((GUI::glfw.height -
							   (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) *
							  GUI::glfw.fb_scale.y),
					  (GLint)(cmd->clip_rect.w * GUI::glfw.fb_scale.x),
					  (GLint)(cmd->clip_rect.h * GUI::glfw.fb_scale.y));
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count,
						   GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count;
		}
		nk_clear(&GUI::glfw.ctx);
	}

	// Default OpenGL state
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);
}

// Init font
void GUI::_deviceUploadAtlas(const void *image, int width, int height) {
	struct device *dev = &GUI::glfw.ogl;
	glGenTextures(1, &dev->font_tex);
	glBindTexture(GL_TEXTURE_2D, dev->font_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void GUI::_nkFontStashBegin() {
	nk_font_atlas_init_default(GUI::glfw.atlas);
	nk_font_atlas_begin(GUI::glfw.atlas);
}

void GUI::_nkFontStashEnd() {
	const void *image;
	int w, h;
	image = nk_font_atlas_bake(GUI::glfw.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
	_deviceUploadAtlas(image, w, h);
	nk_font_atlas_end(GUI::glfw.atlas,
					  nk_handle_id((int)GUI::glfw.ogl.font_tex),
					  &GUI::glfw.ogl.null);
	if (GUI::glfw.atlas->default_font && _media->myFonts.size() == 0)
		nk_style_set_font(&GUI::glfw.ctx,
						  &GUI::glfw.atlas->default_font->handle);
	else if (_media->myFonts.size())
		nk_style_set_font(&GUI::glfw.ctx,
						  &_media->myFonts.begin()->second->handle);
}

struct nk_image GUI::iconLoad(const char *filename) {
	int x, y, n;
	GLuint tex;
	unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	if (!data) {
		std::cerr << "Image not found at " << filename << std::endl;
		filename = NULL;
		return nk_image_id(-1);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
					GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				 data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return nk_image_id((int)tex);
}

void GUI::_setFonts(
	std::vector<std::tuple<float, std::string, std::string>> &vFontPath) {
	struct nk_font *tmpFont;
	for (const auto &font : vFontPath) {
		tmpFont = nk_font_atlas_add_from_file(
			GUI::glfw.atlas, std::get<1>(font).c_str(), std::get<0>(font), 0);
		if (tmpFont)
			_media->myFonts[std::to_string((int)std::get<0>(font)) + "_" +
							std::get<2>(font)] = tmpFont;
	}
}

void GUI::_setImages(
	std::vector<std::tuple<std::string, std::string>> &vImagePath) {
	glEnable(GL_TEXTURE_2D);
	struct nk_image tmpImage;
	for (const auto &img : vImagePath) {
		if (_media->myImages.find(std::get<1>(img)) == _media->myImages.end()) {
			tmpImage = iconLoad(std::get<0>(img).c_str());
			_media->myImages[std::get<1>(img)] = tmpImage;
		}
	}
}

// Init nuklear for use GUI
void GUI::_nkInit(GLFWwindow *win) {
	GUI::glfw.win = win;
	glfwSetScrollCallback(win, GUI::nkScrollCallback);
	glfwSetCharCallback(win, GUI::nkCharCallback);
	glfwSetMouseButtonCallback(win, GUI::nkMouseButtonCallback);
	nk_init_default(&GUI::glfw.ctx, 0);
	GUI::glfw.ctx.clip.copy = GUI::nkClipboardCopy;
	GUI::glfw.ctx.clip.paste = GUI::nkClipboardPaste;
	GUI::glfw.ctx.clip.userdata = nk_handle_ptr(0);
	GUI::glfw.last_button_click = 0;
	_nkDeviceCreate();

	GUI::glfw.is_double_click_down = nk_false;
	GUI::glfw.double_click_pos = nk_vec2(0, 0);
}

void GUI::nkClipboardPaste(nk_handle usr, struct nk_text_edit *edit) {
	const char *text = glfwGetClipboardString(GUI::glfw.win);
	if (text) nk_textedit_paste(edit, text, nk_strlen(text));
	(void)usr;
}

void GUI::nkClipboardCopy(nk_handle usr, const char *text, int len) {
	char *str = 0;
	(void)usr;
	if (!len) return;
	str = (char *)malloc((size_t)len + 1);
	if (!str) return;
	memcpy(str, text, (size_t)len);
	str[len] = '\0';
	glfwSetClipboardString(GUI::glfw.win, str);
	free(str);
}

void GUI::nkCharCallback(GLFWwindow *win, unsigned int codepoint) {
	(void)win;
	if (GUI::glfw.text_len < NK_GLFW_TEXT_MAX)
		GUI::glfw.text[GUI::glfw.text_len++] = codepoint;
}

void GUI::nkScrollCallback(GLFWwindow *win, double xoff, double yoff) {
	(void)win;
	(void)xoff;
	GUI::glfw.scroll.x += (float)xoff;
	GUI::glfw.scroll.y += (float)yoff;
}

void GUI::nkMouseButtonCallback(GLFWwindow *window, int button, int action,
								int mods) {
	(void)mods;
	double x, y;
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;
	glfwGetCursorPos(window, &x, &y);
	if (action == GLFW_PRESS) {
		double dt = glfwGetTime() - GUI::glfw.last_button_click;
		if (dt > NK_GLFW_DOUBLE_CLICK_LO && dt < NK_GLFW_DOUBLE_CLICK_HI) {
			GUI::glfw.is_double_click_down = nk_true;
			GUI::glfw.double_click_pos = nk_vec2((float)x, (float)y);
		}
		GUI::glfw.last_button_click = glfwGetTime();
	} else
		GUI::glfw.is_double_click_down = nk_false;
}

void GUI::_nkDeviceCreate(void) {
	GLint status;
	static const GLchar *vertex_shader = NK_SHADER_VERSION
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 TexCoord;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main() {\n"
		"   Frag_UV = TexCoord;\n"
		"   Frag_Color = Color;\n"
		"   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
		"}\n";
	static const GLchar *fragment_shader = NK_SHADER_VERSION
		"precision mediump float;\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main(){\n"
		"   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	struct device *dev = &GUI::glfw.ogl;
	nk_buffer_init_default(&dev->cmds);
	dev->prog = glCreateProgram();
	dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
	dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
	glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
	glCompileShader(dev->vert_shdr);
	glCompileShader(dev->frag_shdr);
	glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glAttachShader(dev->prog, dev->vert_shdr);
	glAttachShader(dev->prog, dev->frag_shdr);
	glLinkProgram(dev->prog);
	glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
	assert(status == GL_TRUE);

	dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
	dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
	dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
	dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
	dev->attrib_col = glGetAttribLocation(dev->prog, "Color");

	{
		// Buffer setup
		GLsizei vs = sizeof(struct nk_vertex);
		size_t vp = offsetof(struct nk_vertex, position);
		size_t vt = offsetof(struct nk_vertex, uv);
		size_t vc = offsetof(struct nk_vertex, col);

		glGenBuffers(1, &dev->vbo);
		glGenBuffers(1, &dev->ebo);
		glGenVertexArrays(1, &dev->vao);

		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glEnableVertexAttribArray((GLuint)dev->attrib_pos);
		glEnableVertexAttribArray((GLuint)dev->attrib_uv);
		glEnableVertexAttribArray((GLuint)dev->attrib_col);

		glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE,
							  vs, (void *)vp);
		glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs,
							  (void *)vt);
		glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE,
							  GL_TRUE, vs, (void *)vc);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Free GUI (nuklear)
void GUI::_nkDeviceDestroy() {
	struct device *dev = &GUI::glfw.ogl;
	glDetachShader(dev->prog, dev->vert_shdr);
	glDetachShader(dev->prog, dev->frag_shdr);
	glDeleteShader(dev->vert_shdr);
	glDeleteShader(dev->frag_shdr);
	glDeleteProgram(dev->prog);
	glDeleteTextures(1, &dev->font_tex);
	glDeleteVertexArrays(1, &dev->vao);
	glDeleteBuffers(1, &dev->vbo);
	glDeleteBuffers(1, &dev->ebo);
	nk_buffer_free(&dev->cmds);
}

void GUI::_nkShutdown() {
	nk_font_atlas_clear(GUI::glfw.atlas);
	nk_free(&GUI::glfw.ctx);
	_nkDeviceDestroy();
}

/*
** Style
*/

void GUI::getDefaultStyle(enum theme theme, std::map<int, nk_color> *styleMap) {
	if (theme == THEME_WHITE) {
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_TEXT, nk_rgba(70, 70, 70, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_WINDOW,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_HEADER,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_BORDER, nk_rgba(0, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON,
												  nk_rgba(185, 185, 185, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_HOVER,
												  nk_rgba(170, 170, 170, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_ACTIVE,
												  nk_rgba(160, 160, 160, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE,
												  nk_rgba(150, 150, 150, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_HOVER,
												  nk_rgba(120, 120, 120, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_CURSOR,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT,
												  nk_rgba(190, 190, 190, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT_ACTIVE,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER,
												  nk_rgba(190, 190, 190, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR,
												  nk_rgba(80, 80, 80, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_HOVER,
												  nk_rgba(70, 70, 70, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_ACTIVE,
												  nk_rgba(60, 60, 60, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_PROPERTY,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT,
												  nk_rgba(150, 150, 150, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT_CURSOR,
												  nk_rgba(0, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_COMBO,
												  nk_rgba(175, 175, 175, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART,
												  nk_rgba(160, 160, 160, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART_COLOR,
												  nk_rgba(45, 45, 45, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_CHART_COLOR_HIGHLIGHT, nk_rgba(255, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR,
												  nk_rgba(180, 180, 180, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR_CURSOR,
												  nk_rgba(140, 140, 140, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_HOVER, nk_rgba(150, 150, 150, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, nk_rgba(160, 160, 160, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TAB_HEADER,
												  nk_rgba(180, 180, 180, 255)));
	} else if (theme == THEME_RED) {
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TEXT,
												  nk_rgba(190, 190, 190, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_WINDOW,
												  nk_rgba(30, 33, 40, 215)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_HEADER,
												  nk_rgba(181, 45, 69, 220)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BORDER,
												  nk_rgba(51, 55, 67, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON,
												  nk_rgba(181, 45, 69, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_HOVER,
												  nk_rgba(190, 50, 70, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_ACTIVE,
												  nk_rgba(195, 55, 75, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE,
												  nk_rgba(51, 55, 67, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_HOVER,
												  nk_rgba(45, 60, 60, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_CURSOR,
												  nk_rgba(181, 45, 69, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT,
												  nk_rgba(51, 55, 67, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT_ACTIVE,
												  nk_rgba(181, 45, 69, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER,
												  nk_rgba(51, 55, 67, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR,
												  nk_rgba(181, 45, 69, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_HOVER,
												  nk_rgba(186, 50, 74, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_ACTIVE,
												  nk_rgba(191, 55, 79, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_PROPERTY,
												  nk_rgba(51, 55, 67, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_EDIT, nk_rgba(51, 55, 67, 225)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT_CURSOR,
												  nk_rgba(190, 190, 190, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_COMBO, nk_rgba(51, 55, 67, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_CHART, nk_rgba(51, 55, 67, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART_COLOR,
												  nk_rgba(170, 40, 60, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_CHART_COLOR_HIGHLIGHT, nk_rgba(255, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR,
												  nk_rgba(30, 33, 40, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR_CURSOR,
												  nk_rgba(64, 84, 95, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_HOVER, nk_rgba(70, 90, 100, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, nk_rgba(75, 95, 105, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TAB_HEADER,
												  nk_rgba(181, 45, 69, 220)));
	} else if (theme == THEME_BLUE) {
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_TEXT, nk_rgba(20, 20, 20, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_WINDOW,
												  nk_rgba(202, 212, 214, 215)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_HEADER,
												  nk_rgba(137, 182, 224, 220)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BORDER,
												  nk_rgba(140, 159, 173, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON,
												  nk_rgba(137, 182, 224, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_HOVER,
												  nk_rgba(142, 187, 229, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_ACTIVE,
												  nk_rgba(147, 192, 234, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE,
												  nk_rgba(177, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_HOVER,
												  nk_rgba(182, 215, 215, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_CURSOR,
												  nk_rgba(137, 182, 224, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT,
												  nk_rgba(177, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT_ACTIVE,
												  nk_rgba(137, 182, 224, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER,
												  nk_rgba(177, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR,
												  nk_rgba(137, 182, 224, 245)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_HOVER,
												  nk_rgba(142, 188, 229, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_ACTIVE,
												  nk_rgba(147, 193, 234, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_PROPERTY,
												  nk_rgba(210, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT,
												  nk_rgba(210, 210, 210, 225)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT_CURSOR,
												  nk_rgba(20, 20, 20, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_COMBO,
												  nk_rgba(210, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART,
												  nk_rgba(210, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART_COLOR,
												  nk_rgba(137, 182, 224, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_CHART_COLOR_HIGHLIGHT, nk_rgba(255, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR,
												  nk_rgba(190, 200, 200, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR_CURSOR,
												  nk_rgba(64, 84, 95, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_HOVER, nk_rgba(70, 90, 100, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, nk_rgba(75, 95, 105, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TAB_HEADER,
												  nk_rgba(156, 193, 220, 255)));
	} else if (theme == THEME_DARK) {
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TEXT,
												  nk_rgba(210, 210, 210, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_WINDOW,
												  nk_rgba(57, 67, 71, 215)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_HEADER,
												  nk_rgba(51, 51, 56, 220)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BORDER,
												  nk_rgba(46, 46, 46, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON,
												  nk_rgba(48, 83, 111, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_HOVER,
												  nk_rgba(58, 93, 121, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_BUTTON_ACTIVE,
												  nk_rgba(63, 98, 126, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE,
												  nk_rgba(50, 58, 61, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_HOVER,
												  nk_rgba(45, 53, 56, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TOGGLE_CURSOR,
												  nk_rgba(48, 83, 111, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT,
												  nk_rgba(57, 67, 61, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SELECT_ACTIVE,
												  nk_rgba(48, 83, 111, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER,
												  nk_rgba(50, 58, 61, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR,
												  nk_rgba(48, 83, 111, 245)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_HOVER,
												  nk_rgba(53, 88, 116, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SLIDER_CURSOR_ACTIVE,
												  nk_rgba(58, 93, 121, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_PROPERTY,
												  nk_rgba(50, 58, 61, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_EDIT, nk_rgba(50, 58, 61, 225)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_EDIT_CURSOR,
												  nk_rgba(210, 210, 210, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_COMBO, nk_rgba(50, 58, 61, 255)));
		styleMap->insert(
			std::pair<int, nk_color>(NK_COLOR_CHART, nk_rgba(50, 58, 61, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_CHART_COLOR,
												  nk_rgba(48, 83, 111, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_CHART_COLOR_HIGHLIGHT, nk_rgba(255, 0, 0, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR,
												  nk_rgba(50, 58, 61, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_SCROLLBAR_CURSOR,
												  nk_rgba(48, 83, 111, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_HOVER, nk_rgba(53, 88, 116, 255)));
		styleMap->insert(std::pair<int, nk_color>(
			NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, nk_rgba(58, 93, 121, 255)));
		styleMap->insert(std::pair<int, nk_color>(NK_COLOR_TAB_HEADER,
												  nk_rgba(48, 83, 111, 255)));
	}
}

void GUI::setStyle(std::map<int, nk_color> &styleMap) {
	nk_color table[NK_COLOR_COUNT];
	for (const auto &style : styleMap) {
		table[style.first] = style.second;
	}
	nk_style_from_table(&glfw.ctx, table);
}

void GUI::setAssetImage(
	std::vector<std::tuple<std::string, std::string>> vFontImage) {
	if (!vFontImage.empty()) _setImages(vFontImage);
}

bool GUI::uiStartBlock(const char *id, const char *title, struct nk_rect bounds,
					   nk_flags flags) {
	if (nk_begin_titled(&GUI::glfw.ctx, id, title, bounds, flags)) return true;
	return false;
}

void GUI::uiEndBlock() { nk_end(&GUI::glfw.ctx); }

void GUI::uiApplyFont(std::string fontName) {
	if (_media->myFonts.find(fontName) != _media->myFonts.end())
		nk_style_set_font(&glfw.ctx, &_media->myFonts[fontName]->handle);
}

void GUI::uiApplyDefaultFont() {
	if (_media->myFonts.find(_media->defaultFont) != _media->myFonts.end())
		nk_style_set_font(&glfw.ctx,
						  &_media->myFonts[_media->defaultFont]->handle);
}

void GUI::uiSetDefaultFont(std::string fontName) {
	_media->defaultFont = fontName;
	uiApplyDefaultFont();
}

void GUI::uiSetImage(std::string imgName) {
	if (_media->myImages.find(imgName) != _media->myImages.end())
		nk_image(&glfw.ctx, _media->myImages.at(imgName));
}

void GUI::uiHeader(const char *title, nk_flags flags, int rowSize,
				   std::string fontName) {
	uiApplyFont(fontName);
	nk_layout_row_dynamic(&glfw.ctx, rowSize, 1);
	nk_label(&glfw.ctx, title, flags);
	uiApplyDefaultFont();
}

void GUI::uiText(std::string text, nk_flags textFlags, std::string fontName) {
	uiApplyFont(fontName);
	nk_label(&glfw.ctx, text.c_str(), textFlags);
	uiApplyDefaultFont();
}

void GUI::uiWidget(float height, std::string fontName) {
	static const float ratio[] = {0.15f, 0.85f};
	uiApplyFont(fontName);
	nk_layout_row(&glfw.ctx, NK_DYNAMIC, height, 2, ratio);
	nk_spacing(&glfw.ctx, 1);
	uiApplyDefaultFont();
}

bool GUI::uiButton(int width, int height, nk_flags flags, std::string text,
				   std::string imgName, std::string fontName, bool isAlone) {
	width -= 15;
	height -= 10;
	if (isAlone) {
		nk_layout_row_begin(&glfw.ctx, NK_STATIC, height, 1);
		nk_layout_row_push(&glfw.ctx, width);
	}
	uiApplyFont(fontName);
	if (imgName.compare("") != 0 && text.compare("") != 0 &&
		_media->myImages.find(imgName) != _media->myImages.end()) {
		if (nk_button_image_label(&glfw.ctx, _media->myImages.at(imgName),
								  text.c_str(), flags))
			return true;
	} else if (imgName.compare("") != 0 &&
			   _media->myImages.find(imgName) != _media->myImages.end()) {
		if (nk_button_image(&glfw.ctx, _media->myImages.at(imgName)))
			return true;
	} else {
		if (nk_button_label(&glfw.ctx, text.c_str())) return true;
	}
	if (isAlone) nk_layout_row_end(&glfw.ctx);
	uiApplyDefaultFont();
	return false;
}

void GUI::uiWidgetCentered(float height, std::string fontName) {
	static const float ratio[] = {0.15f, 0.50f, 0.35f};
	uiApplyFont(fontName);
	nk_layout_row(&glfw.ctx, NK_DYNAMIC, height, 3, ratio);
	nk_spacing(&glfw.ctx, 1);
	uiApplyDefaultFont();
}

void GUI::uiDialogBox(const char *name, std::string imgName, const char *text,
					  bool isImgLeft, size_t maxCharPerLine, int nbrOfLine,
					  nk_flags textPosition, std::string fontText,
					  std::string fontTitle) {
	struct nk_rect rect =
		nk_rect(0, (WINDOW_H / 4) * 3, WINDOW_W, WINDOW_H / 4);
	int imgWidth = (WINDOW_H / 4) - 40;
	int dialogBoxHeight = (WINDOW_H / 4) - 45;
	int textWidth = (WINDOW_W / 4) * 3 - (imgWidth - (WINDOW_W / 4)) - 40;
	if (uiStartBlock("dialog", name, rect, 0)) {
		uiHeader(name, NK_TEXT_CENTERED, 20, fontTitle);
		uiApplyDefaultFont();
		uiApplyFont(fontText);
		nk_layout_row_begin(&glfw.ctx, NK_STATIC, dialogBoxHeight, 2);
		if (isImgLeft) {
			nk_layout_row_push(&glfw.ctx, imgWidth);
			uiSetImage(imgName);
		}
		nk_layout_row_push(&glfw.ctx, textWidth);
		if (nk_group_begin(&glfw.ctx, "dialogLines",
						   NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {
			size_t printedChars = 0;
			std::string myStr = std::string(text);
			if ((dialogBoxHeight / 20) - 1 < nbrOfLine || nbrOfLine <= 0)
				nbrOfLine = (dialogBoxHeight / 20) - 1;
			nk_layout_row_dynamic(&glfw.ctx,
								  (dialogBoxHeight / nbrOfLine) - 1.0f, 1);
			for (int i = 0; i < nbrOfLine; i++) {
				std::string partialStr =
					myStr.substr(printedChars, maxCharPerLine);
				if (partialStr.size() == maxCharPerLine) {
					std::size_t found = partialStr.rfind(" ");
					if (found != std::string::npos)
						partialStr = partialStr.substr(0, found + 1);
				}
				nk_label(&glfw.ctx, partialStr.c_str(), textPosition);
				printedChars += partialStr.size();
			}
			nk_group_end(&glfw.ctx);
		}
		if (!isImgLeft) {
			nk_layout_row_push(&glfw.ctx, imgWidth);
			uiSetImage(imgName);
		}
		nk_layout_row_end(&glfw.ctx);
	}
	uiEndBlock();
	uiApplyDefaultFont();
}

bool GUI::uiHorizontalSelection(int widgetWidth, std::string leftText,
								std::string rightText, int *choice,
								int maxSize) {
	bool tmp = false;
	widgetWidth -= 100;
	nk_layout_row_begin(&glfw.ctx, NK_STATIC, 30, 4);
	if (leftText.size() != 0) nk_layout_row_push(&glfw.ctx, widgetWidth / 2);
	nk_label(&glfw.ctx, leftText.c_str(), NK_TEXT_LEFT);
	nk_layout_row_push(&glfw.ctx, 30);
	if (_media->myImages.find("leftAngleBracket") != _media->myImages.end()) {
		if (nk_button_image(&glfw.ctx,
							_media->myImages.at("leftAngleBracket"))) {
			if (*choice > 0) *choice -= 1;
			tmp = true;
		}
	}
	if (leftText.size() != 0)
		nk_layout_row_push(&glfw.ctx, widgetWidth / 2);
	else
		nk_layout_row_push(&glfw.ctx, widgetWidth);
	nk_label(&glfw.ctx, rightText.c_str(), NK_TEXT_CENTERED);
	nk_layout_row_push(&glfw.ctx, 30);
	if (_media->myImages.find("rightAngleBracket") != _media->myImages.end()) {
		if (nk_button_image(&glfw.ctx,
							_media->myImages.at("rightAngleBracket"))) {
			if (*choice < maxSize) *choice += 1;
			tmp = true;
		}
	}
	nk_layout_row_end(&glfw.ctx);
	return tmp;
}

bool GUI::uiHover() {
	if (nk_window_is_hovered(&glfw.ctx)) return true;
	return false;
}

void GUI::uiHorizontalEditString(int widgetWidth, std::string leftText,
								 nk_flags flags, char *fieldBuffer, int *len,
								 int max, nk_plugin_filter filter) {
	widgetWidth -= 100;
	nk_layout_row_begin(&glfw.ctx, NK_STATIC, 30, 2);
	nk_layout_row_push(&glfw.ctx, widgetWidth);
	nk_label(&glfw.ctx, leftText.c_str(), NK_TEXT_LEFT);
	nk_layout_row_push(&glfw.ctx, 50);
	uiEditString(flags, fieldBuffer, len, max, filter);
	nk_layout_row_end(&glfw.ctx);
}

void GUI::uiEditString(nk_flags flags, char *fieldBuffer, int *len, int max,
					   nk_plugin_filter filter) {
	nk_edit_string(&glfw.ctx, flags, fieldBuffer, len, max, filter);
}

void GUI::uiRowMultipleElem(bool isSart, int height, int nbrOfElem,
							nk_layout_format flags) {
	if (isSart) {
		nk_layout_row_begin(&glfw.ctx, flags, height, nbrOfElem);
		return;
	}
	nk_layout_row_end(&glfw.ctx);
}

void GUI::uiAddElemInRow(int width) { nk_layout_row_push(&glfw.ctx, width); }

void GUI::uiAddElemOffset(int offset) {
	glfw.ctx.current->layout->row.item_offset +=
		offset;  // Do not have any other way to change offset
}
