#if defined(WIN32)
#include <windows.h>
#endif

#include <GL/gl3w.h>
#include <SDL.h>

#include <stdio.h>

#include "data.h"

#define WIDTH 800
#define HEIGHT 600
#define ZOOM 32

void init();
void update(float dt);
void render();

GLuint g_program;
GLuint g_mapVB;

#if defined(WIN32)
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
#else
int main(int argc, char** argv)
#endif
{
    // Init SDL
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    auto sdlWindow = SDL_CreateWindow(
        "Data Oriented Tower Defense",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    
    auto glContext = SDL_GL_CreateContext(sdlWindow);
    SDL_GL_SetSwapInterval(1);
    gl3wInit();

    init();

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    // event.key.keysym.sym
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEMOTION:
                    break;
            }
        }

        // Update game
        update(0.0f);

        // Draw game
        render();

        // Swap buffers
        SDL_GL_SwapWindow(sdlWindow);
    }

    // Cleanup
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void init()
{
    // Load shader
    const GLchar* vertex_shader_with_version[2] = {
        "#version 130\n",
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec4 Color;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_Color = Color;\n"
        "    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n"
    };
    auto vertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertHandle, 2, vertex_shader_with_version, NULL);
    glCompileShader(vertHandle);

    const GLchar* fragment_shader_with_version[2] = {
        "#version 130\n",
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "    Out_Color = Frag_Color;\n"
        "}\n"
    };
    auto fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragHandle, 2, fragment_shader_with_version, NULL);
    glCompileShader(fragHandle);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertHandle);
    glAttachShader(program, fragHandle);
    glLinkProgram(program);
    glUseProgram(program);
    g_program = program;

    // Set transform matrix into shader
    {
        float L = 0;
        float R = WIDTH;
        float T = 0;
        float B = HEIGHT;
        float S = 2.0f * ZOOM;
        float X = MAP_WIDTH / 2.0f * ZOOM / WIDTH;
        float Y = MAP_HEIGHT / 2.0f * ZOOM / HEIGHT / 4;
        const GLfloat ortho_projection[4][4] =
        {
            { S / (R - L),              0.0f,                   0.0f,   0.0f },
            { 0.0f,                     S / (T - B),            0.0f,   0.0f },
            { 0.0f,                     0.0f,                  -1.0f,   0.0f },
            { (R + L) / (L - R) + X,    (T + B) / (B - T) - Y,  0.0f,   1.0f },
        };
        auto uniform = glGetUniformLocation(program, "ProjMtx");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, &ortho_projection[0][0]);
    }

    // Load map
    glGenBuffers(1, &g_mapVB);
    glBindBuffer(GL_ARRAY_BUFFER, g_mapVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MAP_VERTS), MAP_VERTS, GL_STATIC_DRAW);

    // Set static OpenGL states
    glClearColor(0, 0, 0, 1);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, WIDTH, HEIGHT);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // color
}

void update(float dt)
{
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw map
    glBindBuffer(GL_ARRAY_BUFFER, g_mapVB);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 24, (float*)(uintptr_t)(0));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 24, (float*)(uintptr_t)(8));
    glDrawArrays(GL_LINES, 0, sizeof(MAP_VERTS) / (sizeof(float) * 6));
}
