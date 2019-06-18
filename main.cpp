#include "hello.h"
#include <GL/glew.h>
#include <SDL.h> 
#include <iostream>

int initialize();
int update();
int cleanup();

SDL_Window    *m_window;
SDL_GLContext  m_context;

int initialize()
{
    // Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL video" << std::endl;
        return 1;
    }

    // Create main window
    m_window = SDL_CreateWindow(
        "Hello CUDA OpenGL SDL2!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_OPENGL);
    if (m_window == NULL) {
        std::cerr << "Failed to create main window" << std::endl;
        SDL_Quit();
        return 1;
    }

    // Initialize rendering context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_context = SDL_GL_CreateContext(m_window);
    if (m_context == NULL) {
        std::cerr << "Failed to create GL context" << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1); // Use VSYNC

    // Initialize GL Extension Wrangler (GLEW)
    GLenum err;
    glewExperimental = GL_TRUE; // Please expose OpenGL 3.x+ interfaces
    err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to init GLEW" << std::endl;
        SDL_GL_DeleteContext(m_context);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return 1;
    }

    return 0;
}

int update()
{
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // draw moar here
    SDL_GL_SwapWindow(m_window);
    return 0;
}

int cleanup()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}

int main(int argc, char *argv[])
{

    std::cout << "Hello from C++!" << std::endl;

    hello(); // hello from CUDA

    std::cout << "Initializing SDL & OpenGL" << std::endl;
    if (initialize()) {
        return 1;
    }

    std::cout << "Running..." << std::endl;
    int should_run = 1;
    for (should_run = 1; should_run; ) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                should_run = 0;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    should_run = 0;
                }
            }
        }

        update();
    }

    std::cout << "Exiting..." << std::endl;
    cleanup();
    return 0;
}