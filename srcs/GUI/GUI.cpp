#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "GUI/GUI.hpp"

GUI::GUI() {
	return ;
}

static void
device_upload_atlas(struct device *dev, const void *image, int width, int height)
{
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, image);
}

GUI::GUI(GLFWwindow *window) : _win(window) {
	/*
	** V2 start
	*/
	// std::cout << "" << std::endl;
	// struct nk_font_atlas atlas;
	glfwSetWindowUserPointer(_win, &_ctx);
	struct nk_font_atlas _atlas;
	{
		std::cout << "1" << std::endl;
		_deviceInit(&_device);
		std::cout << "2" << std::endl;
		{
			// std::cout << "1" << std::endl;
			const void *image;
			int w = 0;
			int h = 0;
		    struct nk_font_config cfg = nk_font_config(0);
		    cfg.oversample_h = 3;
			cfg.oversample_v = 2;
		    nk_font_atlas_init_default(&_atlas);
		    nk_font_atlas_begin(&_atlas);
		    _media.font_14 = nk_font_atlas_add_from_file(&_atlas, "~/Desktop/AmaticSC-Regular.ttf", 14.0f, &cfg);
		    _media.font_18 = nk_font_atlas_add_from_file(&_atlas, "~/Desktop/AmaticSC-Regular.ttf", 18.0f, &cfg);
		    _media.font_20 = nk_font_atlas_add_from_file(&_atlas, "~/Desktop/AmaticSC-Regular.ttf", 20.0f, &cfg);
		    _media.font_22 = nk_font_atlas_add_from_file(&_atlas, "~/Desktop/AmaticSC-Regular.ttf", 22.0f, &cfg);
		    image = nk_font_atlas_bake(&_atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
		    device_upload_atlas(&_device, image, w, h);
		    nk_font_atlas_end(&_atlas, nk_handle_id((int)_device.font_tex), &_device.null);
			// std::cout << "2" << std::endl;
		}
	// 	// std::cout << "3" << std::endl;
	    nk_init_default(&_ctx, &_media.font_14->handle);
	// 	// std::cout << "4" << std::endl;
	}
	std::cout << "5" << std::endl;
	/*
	** V2 end
	*/

	/*
	** V1 start
	*/

	// ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
	// nk_glfw3_font_stash_begin(&atlas);
	// nk_glfw3_font_stash_end();
	// bg.r = 0.0f;
	// bg.g = 0.0f;
	// bg.b = 0.0f;
	// bg.a = 0.0f;

	/*
	** V1 end
	*/
}

GUI::~GUI() {

}

// GUI &GUI::operator=(GUI const &rhs) {
// 	*this = rhs;
// 	return *this;
// }

void GUI::drawGUI() {
}

void GUI::_deviceInit(struct device *dev)
{
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
	std::cout << "A" << std::endl;
    nk_buffer_init_default(&dev->cmds);
	std::cout << "A.1" << std::endl;
    dev->prog = glCreateProgram();
	std::cout << "A.2" << std::endl;
    dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
	std::cout << "A.3" << std::endl;
	dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "A.4" << std::endl;
    glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
    glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
    glCompileShader(dev->vert_shdr);
    glCompileShader(dev->frag_shdr);
    glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
	std::cout << "A.5" << std::endl;
    assert(status == GL_TRUE);
    glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);
    glAttachShader(dev->prog, dev->vert_shdr);
    glAttachShader(dev->prog, dev->frag_shdr);
    glLinkProgram(dev->prog);
    glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
    assert(status == GL_TRUE);
	std::cout << "B" << std::endl;
    dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
    dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
    dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
    dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
    dev->attrib_col = glGetAttribLocation(dev->prog, "Color");
	std::cout << "C" << std::endl;
    {
        /* buffer setup */
        GLsizei vs = sizeof(struct nk_glfw_vertex);
        size_t vp = offsetof(struct nk_glfw_vertex, position);
        size_t vt = offsetof(struct nk_glfw_vertex, uv);
        size_t vc = offsetof(struct nk_glfw_vertex, col);

        glGenBuffers(1, &dev->vbo);
        glGenBuffers(1, &dev->ebo);
        glGenVertexArrays(1, &dev->vao);

        glBindVertexArray(dev->vao);
        glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

        glEnableVertexAttribArray((GLuint)dev->attrib_pos);
        glEnableVertexAttribArray((GLuint)dev->attrib_uv);
        glEnableVertexAttribArray((GLuint)dev->attrib_col);

        glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}





/*
** V1 start
*/

// void GUI::drawGUI() {
// 	nk_glfw3_new_frame();
//
// 	/* GUI */
// 	struct nk_rect rect = nk_rect(50, 50, 430, 550);
// 	if (nk_begin_titled(ctx, "Demo", "Toto", rect,
// 				 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
// 					 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
// 	{
// 		enum
// 		{
// 			EASY,
// 			HARD
// 		};
// 		static int op = EASY;
// 		static int property = 20;
// 		nk_layout_row_static(ctx, 30, 80, 1);
// 		if (nk_button_label(ctx, "button"))
// 			fprintf(stdout, "button pressed\n");
//
// 		nk_layout_row_dynamic(ctx, 30, 2);
// 		if (nk_option_label(ctx, "easy", op == EASY))
// 			op = EASY;
// 		if (nk_option_label(ctx, "hard", op == HARD))
// 			op = HARD;
//
// 		nk_layout_row_dynamic(ctx, 25, 1);
// 		nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
//
// 		nk_layout_row_dynamic(ctx, 20, 1);
// 		nk_label(ctx, "background:", NK_TEXT_LEFT);
// 		nk_layout_row_dynamic(ctx, 25, 1);
// 		if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400)))
// 		{
// 			nk_layout_row_dynamic(ctx, 120, 1);
// 			bg = nk_color_picker(ctx, bg, NK_RGBA);
// 			nk_layout_row_dynamic(ctx, 25, 1);
// 			bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
// 			bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
// 			bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
// 			bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
// 			nk_combo_end(ctx);
// 		}
// 		if (nk_window_is_hovered(ctx))
// 			std::cout << "hover" << std::endl;
// 		if (nk_tree_push(ctx, NK_TREE_TAB, "Tree", NK_MINIMIZED)) {
// 			nk_layout_row_dynamic(ctx, 120, 1);
// 		    nk_tree_pop(ctx);
// 		}
// 	}
// 	nk_end(ctx);
// 	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
// }

// void GUI::set_style(struct nk_context *ctx) {
// 	struct nk_color table[NK_COLOR_COUNT];
// 	table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
// 	table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
// 	table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
// 	table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
// 	table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
// 	table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
// 	table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
// 	table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
// 	table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
// 	table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
// 	table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
// 	table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
// 	table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
// 	table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
// 	table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
// 	table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
// 	table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
// 	table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
// 	table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
// 	table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
// 	table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
// 	table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
// 	nk_style_from_table(ctx, table);
// }

/*
** V1 end
*/
