#include "Graphics/GLDebug.h"
#include "Graphics/Shader.h"
#include "Maths.h"
#include <SDL2/SDL.h>
#include <cglm/cam.h>
#include <cglm/struct.h>
#include <glad/glad.h>
#include <nuklear/nuklear_def.h>
#include <nuklear/nuklear_sdl_gl3.h>
#include <stb/stb_image.h>
#include <stdbool.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define WIDTH 1600
#define HEIGHT 900

int main(void)
{
    //=======================================
    //          SDL SET UP SECTION
    //=======================================
    SDL_Init(SDL_INIT_EVERYTHING);

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

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL Nuklear Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH,
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
        struct nk_font_atlas* atlas;
        nk_sdl_font_stash_begin(&atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas,
         * "../../../extra_font/DroidSans.ttf", 14, 0);*/
        nk_sdl_font_stash_end();
    }
    nk_set_style(ctx, THEME_DARK);

    //=======================================
    //          OPENGL OBJECT SETUP
    //=======================================
    // clang-format off
    const GLfloat vertices[] = {

        -0.5f, -0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f, 
         0.5f,  0.5f, 0.0f, 
        -0.5f,  0.5f, 0.0f,
    };

    const GLfloat textureCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };

    const GLuint indices[]  = {
        0, 1, 2, 2, 3, 0
    };
    // clang-format on

    //
    //  Create vertex data
    //
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint textureVbo = 0;
    GLuint ebo = 0;

    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 1);

    // Create VBO, attatch it to the VAO
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(GLfloat) * 12, vertices, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(GLfloat));

    // Create VBO for textures, attatch it to the VAO
    glCreateBuffers(1, &textureVbo);
    glNamedBufferStorage(textureVbo, sizeof(GLfloat) * 8, textureCoords,
                         GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayVertexBuffer(vao, 1, textureVbo, 0, 2 * sizeof(GLfloat));

    // Create EBO, attatch to the VAO
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, sizeof(GLuint) * 6, indices, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao, ebo);

    // Load shaders
    GLuint shader =
        loadShaders("Data/Shaders/MinVertex.glsl", "Data/Shaders/MinFragment.glsl");

    //
    // Load up a texture
    //

    int width;
    int height;
    int channels;
    unsigned char* imageData = stbi_load("Data/Textures/opengl_logo.png", &width, &height,
                                         &channels, STBI_rgb_alpha);

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                        imageData);
    stbi_image_free(imageData);

    //=======================================
    //          OPENGL MISC SETUP
    //=======================================
    // glDisable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 0.0);
    glViewport(0, 0, WIDTH, HEIGHT);

    //=======================================
    //          WOW LETS MAKE IT 3D
    //=======================================
    float aspect = (float)WIDTH / (float)HEIGHT;
    Matrix4 projectionMatrix;
    glm_perspective(glm_rad(90.0f), aspect, 0.1f, 100.0f, projectionMatrix);

    // Camera stuff
    Vector3 playerPosition = VECTOR3_ZERO;
    Vector3 playerRotation = {0, -90.0f, 0.0};
    Vector3 up = {0, 1, 0};
    Vector3 front = VECTOR3_ZERO;

    Vector3 modelLocation = {0, 0, -10};
    Vector3 modelRotation = VECTOR3_ZERO;

    //=======================================
    //          MAIN LOOP
    //=======================================
    const Uint8* keyboard = NULL;
    int lastMouseX = 0;
    int lastMouseY = 0;
    SDL_WarpMouseInWindow(window, WIDTH / 2, HEIGHT / 2);
    SDL_GetMouseState(&lastMouseX, &lastMouseY);

    bool running = true;
    while (running) {
        SDL_Delay(16); //"hack" for 60fps
        nk_input_begin(ctx);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            nk_sdl_handle_event(&event);
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

        //=======================================
        //              INPUT
        //=======================================

        // Keyboard input
        // https://wiki.libsdl.org/SDL_Scancode
        keyboard = SDL_GetKeyboardState(NULL);
        if (keyboard[SDL_SCANCODE_W]) {
            moveVectorForwards(playerPosition, playerRotation, 1);
        }
        if (keyboard[SDL_SCANCODE_A]) {
            moveVectorLeft(playerPosition, playerRotation, 1);
        }
        if (keyboard[SDL_SCANCODE_S]) {
            moveVectorBackwards(playerPosition, playerRotation, 1);
        }
        if (keyboard[SDL_SCANCODE_D]) {
            moveVectorRight(playerPosition, playerRotation, 1);
        }
        //  SDL_SetRelativeMouseMode(SDL_TRUE);

        // Mouse input
        int thisMouseX;
        int thisMouseY;
        Uint32 mouseState = SDL_GetMouseState(&thisMouseX, &thisMouseY);

        int mouseXDiff = thisMouseX - lastMouseX;
        int mouseYDiff = thisMouseY - lastMouseY;

        playerRotation[0] += mouseYDiff / 4.0f;
        playerRotation[1] += mouseXDiff / 4.0f;

        SDL_GetMouseState(&lastMouseX, &lastMouseY);

        // SDL_WarpMouseInWindow(window, WIDTH / 2, HEIGHT / 2);
        // SDL_SetRelativeMouseMode(SDL_FALSE);

        // Update

        // GUI
        if (nk_begin(ctx, "Debug Window", nk_rect(0, 0, 400, 200), 0)) {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_labelf(ctx, NK_STATIC, "Player Position: (%f %f %f)", playerPosition[0],
                      playerPosition[1], playerPosition[2]);
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_labelf(ctx, NK_STATIC, "Player Rotation: (%f %f %f)", playerRotation[0],
                      playerRotation[1], playerRotation[2]);
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_labelf(ctx, NK_STATIC, "Player Front: (%f %f %f)", front[0], front[1],
                      front[2]);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_labelf(ctx, NK_STATIC, "Mouse Position: (%d %d)", lastMouseX, lastMouseY,
                      front[2]);
        }
        nk_end(ctx);

        //=======================================
        //          Render
        //=======================================
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // transform
        Matrix4 modelMatrix = MATRIX4_IDENTITY;
        createModelMatrix(modelLocation, modelRotation, modelMatrix);

        // View Matrix
        Vector3 center;

        front[0] = cos(glm_rad(playerRotation[1])) * cos(glm_rad(playerRotation[0]));
        front[1] = sin(glm_rad(playerRotation[0]));
        front[2] = sin(glm_rad(playerRotation[1])) * cos(glm_rad(playerRotation[0]));
        glm_normalize(front);
        glm_vec3_add(playerPosition, front, center);

        Matrix4 viewMatrix = MATRIX4_IDENTITY;
        glm_lookat(playerPosition, center, up, viewMatrix);

        // Calculate projection view matrix and then upload
        Matrix4 projectionViewMatrix = MATRIX4_IDENTITY;
        glm_mat4_mul(projectionMatrix, viewMatrix, projectionViewMatrix);

        glUseProgram(shader);
        loadMatrix4ToShader(shader, "projectionViewMatrix", projectionViewMatrix);
        loadMatrix4ToShader(shader, "modelMatrix", modelMatrix);

        // Bind stuff then render
        glBindVertexArray(vao);
        glBindTextureUnit(0, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

        SDL_GL_SwapWindow(window);
    }

    //=======================================
    //          CLEAN UP
    //=======================================
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader);
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
