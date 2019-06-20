#include "window.h"
#include <iostream>

void Window::init(const glm::ivec2 &geometry)
{
    this->geometry = geometry;
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_CreateWindowAndRenderer(geometry.x, geometry.y, SDL_WINDOW_SHOWN, &window, &renderer);
    if(window == nullptr || renderer == nullptr)
    {
        std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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
    if(renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if(window != nullptr)
        SDL_DestroyWindow(window);
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

void Window::setPixel(const glm::ivec2 &position)
{
    if(position.x >= geometry.x ||
            position.y >= geometry.y)
        return;

    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void Window::clear()
{
    SDL_RenderClear(renderer);
}
