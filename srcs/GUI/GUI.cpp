#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "GUI/GUI.hpp"

// static void
// grid_demo(struct nk_context *ctx, struct media *media)
// {
//     static char text[3][64];
//     static int text_len[3];
//     static const char *items[] = {"Item 0","item 1","item 2"};
//     static int selected_item = 0;
//     static int check = 1;
//
//     int i;
// 	std::cout << "1" << '\n';
//     nk_style_set_font(ctx, &media->font_20->handle);
// 	std::cout << "2 " << nk_begin(ctx, "Grid Demo", nk_rect(600, 350, 275, 250), NK_WINDOW_NO_SCROLLBAR) << '\n';
//     if (nk_begin(ctx, "Grid Demo", nk_rect(600, 350, 275, 250),
//         NK_WINDOW_TITLE|NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
//         NK_WINDOW_NO_SCROLLBAR))
//     {
// 		std::cout << "2.1" << '\n';
//         nk_style_set_font(ctx, &media->font_18->handle);
//         nk_layout_row_dynamic(ctx, 30, 2);
//         nk_label(ctx, "Floating point:", NK_TEXT_RIGHT);
//         nk_edit_string(ctx, NK_EDIT_FIELD, text[0], &text_len[0], 64, nk_filter_float);
//         nk_label(ctx, "Hexadecimal:", NK_TEXT_RIGHT);
//         nk_edit_string(ctx, NK_EDIT_FIELD, text[1], &text_len[1], 64, nk_filter_hex);
//         nk_label(ctx, "Binary:", NK_TEXT_RIGHT);
//         nk_edit_string(ctx, NK_EDIT_FIELD, text[2], &text_len[2], 64, nk_filter_binary);
//         nk_label(ctx, "Checkbox:", NK_TEXT_RIGHT);
//         nk_checkbox_label(ctx, "Check me", &check);
//         nk_label(ctx, "Combobox:", NK_TEXT_RIGHT);
//         if (nk_combo_begin_label(ctx, items[selected_item], nk_vec2(nk_widget_width(ctx), 200))) {
//             nk_layout_row_dynamic(ctx, 25, 1);
//             for (i = 0; i < 3; ++i)
//                 if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
//                     selected_item = i;
//             nk_combo_end(ctx);
//         }
//     }
// 	std::cout << "3" << '\n';
//     nk_end(ctx);
// 	std::cout << "4" << '\n';
//     nk_style_set_font(ctx, &media->font_14->handle);
// }

GUI::GUI(GLFWwindow *window) : _media(new media()) {
	struct nk_font_atlas tmpAtlas;
	glfw.atlas = &tmpAtlas;
	nkInit(window, NK_GLFW3_INSTALL_CALLBACKS);
	nkFontStashBegin();
	nkFontStashEnd();
	// const void *image;
	// int w = 0;
	// int h = 0;
    // struct nk_font_config cfg = nk_font_config(0);
    // cfg.oversample_h = 3;
	// cfg.oversample_v = 2;
    // nk_font_atlas_init_default(glfw.atlas);
    // nk_font_atlas_begin(glfw.atlas);
    // _media->font_14 = nk_font_atlas_add_from_file(glfw.atlas, "~/Desktop/AmaticSC-Regular.ttf", 14.0f, &cfg);
    // _media->font_18 = nk_font_atlas_add_from_file(glfw.atlas, "~/Desktop/AmaticSC-Regular.ttf", 18.0f, &cfg);
    // _media->font_20 = nk_font_atlas_add_from_file(glfw.atlas, "~/Desktop/AmaticSC-Regular.ttf", 20.0f, &cfg);
    // _media->font_22 = nk_font_atlas_add_from_file(glfw.atlas, "~/Desktop/AmaticSC-Regular.ttf", 22.0f, &cfg);
    // image = nk_font_atlas_bake(glfw.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    // deviceUploadAtlas(image, w, h);
    // nk_font_atlas_end(glfw.atlas, nk_handle_id((int)glfw.ogl.font_tex), &glfw.ogl.null);
	// nk_init_default(&glfw.ctx, &_media->font_14->handle);
}

GUI::~GUI() {
	delete _media;
}

void GUI::drawGUI() {
	nkNewFrame();


	struct nk_rect rect = nk_rect(50, 50, 430, 550);
	if (nk_begin_titled(&glfw.ctx, "Demo", "Toto", rect,
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
		nk_layout_row_static(&glfw.ctx, 30, 80, 1);
		if (nk_button_label(&glfw.ctx, "button"))
			fprintf(stdout, "button pressed\n");

		nk_layout_row_dynamic(&glfw.ctx, 30, 2);
		if (nk_option_label(&glfw.ctx, "easy", op == EASY))
			op = EASY;
		if (nk_option_label(&glfw.ctx, "hard", op == HARD))
			op = HARD;

		nk_layout_row_dynamic(&glfw.ctx, 25, 1);
		nk_property_int(&glfw.ctx, "Compression:", 0, &property, 100, 10, 1);

		nk_layout_row_dynamic(&glfw.ctx, 20, 1);
		nk_label(&glfw.ctx, "background:", NK_TEXT_LEFT);
		nk_layout_row_dynamic(&glfw.ctx, 25, 1);
		if (nk_window_is_hovered(&glfw.ctx))
			std::cout << "hover" << std::endl;
		if (nk_tree_push(&glfw.ctx, NK_TREE_TAB, "Tree", NK_MINIMIZED)) {
			nk_layout_row_dynamic(&glfw.ctx, 120, 1);
		    nk_tree_pop(&glfw.ctx);
		}
	}
	nk_end(&glfw.ctx);


	nkRender(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);;
}

void GUI::nkNewFrame() {
    int i;
    double x, y;
    struct nk_context *ctx = &glfw.ctx;
    struct GLFWwindow *win = glfw.win;

    glfwGetWindowSize(win, &glfw.width, &glfw.height);
    glfwGetFramebufferSize(win, &glfw.display_width, &glfw.display_height);
    glfw.fb_scale.x = (float)glfw.display_width / (float)glfw.width;
    glfw.fb_scale.y = (float)glfw.display_height / (float)glfw.height;

    nk_input_begin(ctx);
    for (i = 0; i < glfw.text_len; ++i)
        nk_input_unicode(ctx, glfw.text[i]);

#ifdef NK_GLFW_GL3_MOUSE_GRABBING
    /* optional grabbing behavior */
    if (ctx->input.mouse.grab)
        glfwSetInputMode(glfw.win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else if (ctx->input.mouse.ungrab)
        glfwSetInputMode(glfw.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif

    nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TEXT_START, glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TEXT_END, glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_SCROLL_START, glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_SCROLL_END, glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_SCROLL_DOWN, glfwGetKey(win, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_SCROLL_UP, glfwGetKey(win, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_SHIFT, glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_UNDO, glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_REDO, glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_LINE_START, glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_TEXT_LINE_END, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
    }
    else {
        nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glfwGetCursorPos(win, &x, &y);
    nk_input_motion(ctx, (int)x, (int)y);
#ifdef NK_GLFW_GL3_MOUSE_GRABBING
    if (ctx->input.mouse.grabbed) {
        glfwSetCursorPos(glfw.win, ctx->input.mouse.prev.x, ctx->input.mouse.prev.y);
        ctx->input.mouse.pos.x = ctx->input.mouse.prev.x;
        ctx->input.mouse.pos.y = ctx->input.mouse.prev.y;
    }
#endif
    nk_input_button(ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_DOUBLE, (int)glfw.double_click_pos.x, (int)glfw.double_click_pos.y, glfw.is_double_click_down);
    nk_input_scroll(ctx, glfw.scroll);
    nk_input_end(&glfw.ctx);
    glfw.text_len = 0;
    glfw.scroll = nk_vec2(0, 0);
}

void GUI::nkRender(enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer) {
    struct device *dev = &glfw.ogl;
    struct nk_buffer vbuf, ebuf;
    GLfloat ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, -2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, -1.0f, 0.0f},
        {-1.0f, 1.0f, 0.0f, 1.0f},
    };
    ortho[0][0] /= (GLfloat)glfw.width;
    ortho[1][1] /= (GLfloat)glfw.height;

    /* setup global state */
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    /* setup program */
    glUseProgram(dev->prog);
    glUniform1i(dev->uniform_tex, 0);
    glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
    glViewport(0, 0, (GLsizei)glfw.display_width, (GLsizei)glfw.display_height);
    {
        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command *cmd;
        void *vertices, *elements;
        const nk_draw_index *offset = NULL;

        /* allocate vertex and element buffer */
        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

        /* load draw vertices & elements directly into vertex + element buffer */
        vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        {
            /* fill convert configuration */
            struct nk_convert_config config;
            static const struct nk_draw_vertex_layout_element vertex_layout[] = {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_vertex, position)},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_vertex, uv)},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_vertex, col)},
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
            config.shape_AA = AA;
            config.line_AA = AA;

            /* setup buffers to load vertices and elements */
            nk_buffer_init_fixed(&vbuf, vertices, (size_t)max_vertex_buffer);
            nk_buffer_init_fixed(&ebuf, elements, (size_t)max_element_buffer);
            nk_convert(&glfw.ctx, &dev->cmds, &vbuf, &ebuf, &config);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, &glfw.ctx, &dev->cmds)
        {
            if (!cmd->elem_count)
                continue;
            glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            glScissor(
                (GLint)(cmd->clip_rect.x * glfw.fb_scale.x),
                (GLint)((glfw.height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * glfw.fb_scale.y),
                (GLint)(cmd->clip_rect.w * glfw.fb_scale.x),
                (GLint)(cmd->clip_rect.h * glfw.fb_scale.y));
            glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
            offset += cmd->elem_count;
        }
        nk_clear(&glfw.ctx);
    }

    /* default OpenGL state */
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
}

/*
** Init font
*/

void GUI::deviceUploadAtlas(const void *image, int width, int height)
{
    struct device *dev = &glfw.ogl;
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void GUI::nkFontStashBegin(/*struct nk_font_atlas **atlas*/)
{
    nk_font_atlas_init_default(glfw.atlas);
    nk_font_atlas_begin(glfw.atlas);
    // *atlas = glfw.atlas;
}

void GUI::nkFontStashEnd()
{
    const void *image;
    int w, h;
    image = nk_font_atlas_bake(glfw.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    deviceUploadAtlas(image, w, h);
    nk_font_atlas_end(glfw.atlas, nk_handle_id((int)glfw.ogl.font_tex), &glfw.ogl.null);
    if (glfw.atlas->default_font)
        nk_style_set_font(&glfw.ctx, &glfw.atlas->default_font->handle);
}

/*
** Init nuklear
*/

struct nk_context * GUI::nkInit(GLFWwindow *win, enum nk_glfw_init_state init_state) {
    glfw.win = win;
    if (init_state == NK_GLFW3_INSTALL_CALLBACKS)
    {
        glfwSetScrollCallback(win, GUI::nkScrollCallback);
        glfwSetCharCallback(win, GUI::nkCharCallback);
        glfwSetMouseButtonCallback(win, GUI::nkMouseButtonCallback);
    }
    nk_init_default(&glfw.ctx, 0);
    glfw.ctx.clip.copy = GUI::nkClipboardCopy;
    glfw.ctx.clip.paste = GUI::nkClipboardPaste;
    glfw.ctx.clip.userdata = nk_handle_ptr(0);
    glfw.last_button_click = 0;
    nkDeviceCreate();

    glfw.is_double_click_down = nk_false;
    glfw.double_click_pos = nk_vec2(0, 0);

    return &glfw.ctx;
}

void GUI::nkClipboardPaste(nk_handle usr, struct nk_text_edit *edit) {
    const char *text = glfwGetClipboardString(glfw.win);
    if (text)
        nk_textedit_paste(edit, text, nk_strlen(text));
    (void)usr;
}

void GUI::nkClipboardCopy(nk_handle usr, const char *text, int len)
{
    char *str = 0;
    (void)usr;
    if (!len)
        return;
    str = (char *)malloc((size_t)len + 1);
    if (!str)
        return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    glfwSetClipboardString(glfw.win, str);
    free(str);
}

void GUI::nkCharCallback(GLFWwindow *win, unsigned int codepoint) {
    (void)win;
    if (glfw.text_len < NK_GLFW_TEXT_MAX)
        glfw.text[glfw.text_len++] = codepoint;
}

void GUI::nkScrollCallback(GLFWwindow *win, double xoff, double yoff) {
    (void)win;
    (void)xoff;
    glfw.scroll.x += (float)xoff;
    glfw.scroll.y += (float)yoff;
}

void GUI::nkMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    (void)mods;
    double x, y;
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;
    glfwGetCursorPos(window, &x, &y);
    if (action == GLFW_PRESS)
    {
        double dt = glfwGetTime() - glfw.last_button_click;
        if (dt > NK_GLFW_DOUBLE_CLICK_LO && dt < NK_GLFW_DOUBLE_CLICK_HI)
        {
            glfw.is_double_click_down = nk_true;
            glfw.double_click_pos = nk_vec2((float)x, (float)y);
        }
        glfw.last_button_click = glfwGetTime();
    }
    else
        glfw.is_double_click_down = nk_false;
}

void GUI::nkDeviceCreate(void) {
    GLint status;
    static const GLchar *vertex_shader =
        NK_SHADER_VERSION
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
    static const GLchar *fragment_shader =
        NK_SHADER_VERSION
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main(){\n"
        "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    struct device *dev = &glfw.ogl;
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
        /* buffer setup */
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

        glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void *)vp);
        glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void *)vt);
        glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void *)vc);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
