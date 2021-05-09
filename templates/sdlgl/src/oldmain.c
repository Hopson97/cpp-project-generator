#define GL_GLEXT_PROTOTYPES

#include "GLDebug.h"
#include <SDL2/SDL.h>
#include <cglm/include/cglm/cglm.h>
#include <cglm/include/cglm/mat4.h>
#include <glad/glad.h>
#include <nuklear/nuklear_def.h>
#include <nuklear/nuklear_sdl_gl3.h>
#include <stdio.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define WIDTH 1600
#define HEIGHT 900

char* getFileContent(const char* fileName)
{
    char* buffer = NULL;
    long length;
    FILE* file = fopen(fileName, "r");

    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) {
            fread(buffer, 1, length, file);
        }
        fclose(file);
    }

    return buffer;
}

/*
static const char* vertex_shader =
    "#version 130\n"
    "in vec2 i_position;\n"
    "in vec4 i_color;\n"
    "out vec4 v_color;\n"
    "uniform mat4 u_projection_matrix;\n"
    "void main() {\n"
    "    v_color = i_color;\n"
    "    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );\n"
    "}\n";

static const char* fragment_shader = "#version 130\n"
                                     "in vec4 v_color;\n"
                                     "out vec4 o_color;\n"
                                     "void main() {\n"
                                     "    o_color = v_color;\n"
                                     "}\n";
*/

int main(void)
{
    //=======================================
    //          SDL SET UP SECTION
    //=======================================
    SDL_Init(SDL_INIT_VIDEO);

    // Set up SDL to work with OpenGL (https://wiki.libsdl.org/SDL_GLattr)
    // Enable hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Set rendering parmeters
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create the window and OpenGL context
    SDL_Window* window =
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH,
                         HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Init OpenGL functions
    if (!gladLoadGL()) {
        printf("Error: Could not load OpenGL.");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    initGLDebug();

    //=======================================
    //          NUKLEAR SET UP SECTION
    //=======================================
    struct nk_context* ctx;
    // set_style(ctx, THEME_WHITE);
    ctx = nk_sdl_init(window);
    {
        // Example of font loading, pretty nice!
        struct nk_font_atlas* atlas;
        nk_sdl_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas,
         * "../../../extra_font/DroidSans.ttf", 14, 0);*/
        nk_sdl_font_stash_end();
    }

    //=======================================
    //          OPENGL OBJECT SETUP
    //=======================================
    const GLfloat vertices[] = {

        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
    };
    GLuint vao = 0;
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    GLuint vbo = 0;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(GLfloat) * 9, vertices, 0);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 0);

    // Load shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();

    char* vertexFile = getFileContent("Data/Shaders/MinVertex.glsl");
    char* fragmentFile = getFileContent("Data/Shaders/MinFragment.glsl");
    if (!vertexFile) {
        fprintf(stderr, "Failed to load vertex file.\n");
        return -1;
    }
    if (!fragmentFile) {
        fprintf(stderr, "Failed to load vertex file.\n");
        return -1;
    }

    int length = strlen(vertexFile);
    glShaderSource(vertexShader, 1, (const GLchar* const*)&vertexFile, &length);
    glCompileShader(vertexShader);

    GLint status = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        fprintf(stderr, "vertex shader compilation failed\n");
        return 1;
    }

    length = strlen(fragmentFile);
    glShaderSource(fragmentShader, 1, (const GLchar* const*)&fragmentFile, &length);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        fprintf(stderr, "fragment shader compilation failed\n");
        return 1;
    }
    free(vertexFile);
    free(fragmentFile);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glBindAttribLocation(program, 0, "inPosition");
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // glDisable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.0, 0.0, 0.0);
    glViewport(0, 0, WIDTH, HEIGHT);

    glUseProgram(program);
    /*
        const GLfloat g_vertex_buffer_data[] = {
            /  R, G, B, A, X, Y  *
            1, 0, 0, 1, 0, 0, 0, 1, 0, 1, width, 0,      0, 0, 1, 1, width, height,

            1, 0, 0, 1, 0, 0, 0, 0, 1, 1, width, height, 1, 1, 1, 1, 0,     height};

        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                     GL_STATIC_DRAW);

        mat4 projection_matrix;
        glm_ortho(0, width, height, 0, 0.0, 100.0, projection_matrix);
        glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1,
       GL_FALSE, &projection_matrix[0][0]);
    */
    // glCre

    //=======================================
    //          MAIN LOOP
    //=======================================
    bool running = true;
    while (running) {
        // nk_input_begin(ctx);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            //    nk_sdl_handle_event(&event);
            switch (event.type) {
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;

                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        /*
                if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                             NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                                 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
                    enum { EASY, HARD };
                    static int op = EASY;
                    static int property = 20;

                    nk_layout_row_static(ctx, 30, 80, 1);
                    if (nk_button_label(ctx, "button"))
                        printf("button pressed!\n");
                    nk_layout_row_dynamic(ctx, 30, 2);
                    if (nk_option_label(ctx, "easy", op == EASY))
                        op = EASY;
                    if (nk_option_label(ctx, "hard", op == HARD))
                        op = HARD;
                    nk_layout_row_dynamic(ctx, 22, 1);
                    nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

                    nk_layout_row_dynamic(ctx, 20, 1);
                    nk_label(ctx, "background:", NK_TEXT_LEFT);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    if (nk_combo_begin_color(ctx, nk_rgb_cf(bg),
                                             nk_vec2(nk_widget_width(ctx), 400))) {
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
        */
        // calculator(ctx);
        // node_editor(ctx);
        // overview(ctx);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //  nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    //=======================================
    //          CLEAN UP
    //=======================================
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
