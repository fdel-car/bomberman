/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jichen-m <jichen-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 11:49:34 by jichen-m          #+#    #+#             */
/*   Updated: 2018/11/07 20:53:02 by jichen-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main()
{
    // GLFWwindow *window;

    // std::cout << "Hello" << std::endl;

    // /* INIT GLFW */
    // if (!glfwInit())
    // {
    //     std::cout << "Failed to initialize GLFW" << std::endl;
    //     return 0;
    // }
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Test GLFW", NULL, NULL); // size of screen will change
    // if (!window)
    // {
    //     glfwTerminate();
    //     std::cout << "Failed to create windows GLFW" << std::endl;
    //     return 0;
    // }
    // const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    // glfwSetWindowPos(window, (mode->width / 2) - (WINDOW_W / 2), (mode->height / 2) - (WINDOW_H / 2));
    // glfwMakeContextCurrent(window);
    // glfwPollEvents();

    // /* NUKLEAR */
    // enum
    // {
    //     EASY,
    //     HARD
    // };
    // static int op = EASY;
    // static float value = 0.6f;
    // // static int i = 20;
    // struct nk_context *ctx;
    // struct nk_font_atlas *atlas;
    // struct nk_font *font;
    // // struct nk_glfw_device ogl;

    // // nk_init_fixed(&ctx, calloc(1, MAX_MEMORY), MAX_MEMORY, 0);
    // nk_sqrt(2.0);
    // nk_cos(2.2);
    // nk_sin(2.2);

    // ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);

    // nk_glfw3_font_stash_begin(&atlas);

    // font = nk_font_atlas_add_from_file(atlas, "./extra_font/ProggyTiny.ttf", 14, 0);

    // nk_glfw3_font_stash_end();

    // // struct nk_glfw_device *dev = &ogl;
    // // glGenTextures(1, &dev->font_tex);
    // // glBindTexture(GL_TEXTURE_2D, dev->font_tex);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
    // //             GL_RGBA, GL_UNSIGNED_BYTE, image);

    // // nk_font_atlas_end(&glfw.atlas, nk_handle_id((int)glfw.ogl.font_tex), &glfw.ogl.null);

    // nk_style_set_font(ctx, &font->handle);

    // if (nk_init_default(ctx, &font->handle) == 0)
    //     std::cout << "FAIL" << std::endl;
    // std::cout << "AA" << std::endl;
    // if (nk_begin(ctx, "Show", nk_rect(50, 50, 220, 220),
    //              NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE))
    // {
    //     // fixed widget pixel width
    //     nk_layout_row_static(ctx, 30, 80, 1);
    //     if (nk_button_label(ctx, "button"))
    //     {
    //         // event handling
    //     }
    //     // fixed widget window ratio width
    //     nk_layout_row_dynamic(ctx, 30, 2);
    //     if (nk_option_label(ctx, "easy", op == EASY))
    //         op = EASY;
    //     if (nk_option_label(ctx, "hard", op == HARD))
    //         op = HARD;
    //     // custom widget pixel width
    //     nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
    //     {
    //         nk_layout_row_push(ctx, 50);
    //         nk_label(ctx, "Volume:", NK_TEXT_LEFT);
    //         nk_layout_row_push(ctx, 110);
    //         nk_slider_float(ctx, 0, &value, 1.0f, 0.1f);
    //     }
    //     nk_layout_row_end(ctx);
    // }

    // /* TO GET SCREEN OF GLFW */
    // // while (!glfwWindowShouldClose(window))
    // // {
    // //     // wipe the drawing surface clear
    // //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // //     // put the stuff we've been drawing onto the display
    // //     glfwSwapBuffers(window);
    // // }
    // for (size_t i = 0; i < 1000000000; i++)
    // {
    // }
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // // nk_end(&ctx);
    // glfwTerminate();
    // return 0;

    /* Platform */
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    /* GLFW */
    // glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    /* OpenGL */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // glewExperimental = 1;
    // if (glewInit() != GLEW_OK)
    // {
    //     fprintf(stderr, "Failed to setup GLEW\n");
    //     exit(1);
    // }

    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
        /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_glfw3_font_stash_end();
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &droid->handle);*/}

#ifdef INCLUDE_STYLE
/*set_style(ctx, THEME_WHITE);*/
/*set_style(ctx, THEME_RED);*/
/*set_style(ctx, THEME_BLUE);*/
/*set_style(ctx, THEME_DARK);*/
#endif

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win))
    {
        /* Input */
        glfwPollEvents();
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

/* -------------- EXAMPLES ---------------- */
#ifdef INCLUDE_CALCULATOR
        calculator(ctx);
#endif
#ifdef INCLUDE_OVERVIEW
        overview(ctx);
#endif
#ifdef INCLUDE_NODE_EDITOR
        node_editor(ctx);
#endif
        /* ----------------------------------------- */

        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}