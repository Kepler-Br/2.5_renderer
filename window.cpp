#include "window.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

void Window::init(const glm::ivec2 &geometry)
{
    this->geometry = geometry;

    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("SDL2 window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    geometry.x, geometry.y,
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cout << "WINDOW::INIT window is nullptr.\nSDL2 error: "
                  << SDL_GetError() << std::endl;
        throw std::runtime_error("Window error.");
    }
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      std::cout << "Error:" << glewGetErrorString(err) << std::endl;
    }

    // Define the viewport dimensions
    glViewport(0, 0, geometry.x, geometry.y);
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    printOpenGLVersion();
}

Window::Window(const glm::ivec2 &geometry, const std::string &windowTitle)
{
    init(geometry);
    setWindowTitle(windowTitle);
}

Window::Window(const int x, const int y, const std::string &windowTitle)
{
    init(glm::ivec2(x, y));
    setWindowTitle(windowTitle);
}

Window::~Window()
{
    if(window != nullptr)
    {
        SDL_DestroyWindow(window);
        SDL_GL_DeleteContext(glContext);
    }
    SDL_Quit();
}

void Window::setWindowTitle(const std::string &title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void Window::setWindowPosition(const glm::ivec2 &position)
{
    SDL_SetWindowPosition(window, position.x, position.y);
}

const glm::ivec2 Window::getGeometry() const
{
    return geometry;
}

void Window::clear(const glm::vec3 &color)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffer()
{
    SDL_GL_SwapWindow(window);
}

void Window::printOpenGLVersion()
{
    int value = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION: " << value << std::endl;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
