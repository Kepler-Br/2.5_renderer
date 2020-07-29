#include "sdl_wrapper.h"
#include<stdexcept>
#include <iostream>

void Sdl_wrapper::init_sdl()
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
        throw std::runtime_error("Unable to init SDL: " + std::string(SDL_GetError()));

    window = SDL_CreateWindow("Wolfenstein", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, SDL_WINDOW_SHOWN);
    if(window == nullptr)
        throw std::runtime_error("Cannot create SDL2 window: " + std::string(SDL_GetError()));


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == nullptr)
        throw std::runtime_error("Cannot create SDL2 renderer: " + std::string(SDL_GetError()));
    this->pixel_count = this->resolution.x * this->resolution.y;
}

Sdl_wrapper::Sdl_wrapper(const glm::ivec2 &resolution) :
    resolution(resolution)
{
    this->init_sdl();
}

SDL_Window *Sdl_wrapper::get_window() const
{
    return this->window;
}

SDL_Renderer *Sdl_wrapper::get_renderer() const
{
    return this->renderer;
}

const glm::ivec2 &Sdl_wrapper::get_resolution() const
{
    return this->resolution;
}

void Sdl_wrapper::set_color(const glm::ivec3 &color)
{
    SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
}

void Sdl_wrapper::set_color(const uint8_t &red, const uint8_t &green, const uint8_t &blue)
{
    SDL_SetRenderDrawColor(this->renderer, red, green, blue, SDL_ALPHA_OPAQUE);
}

void Sdl_wrapper::set_pixel(const glm::ivec2 &position)
{
    SDL_RenderDrawPoint(this->renderer, position.x, position.y);
}

void Sdl_wrapper::put_filled_rect(const glm::ivec2 &position, const glm::ivec2 &dimensions)
{
    SDL_Rect rect = {position.x, position.y, dimensions.x, dimensions.y};
    SDL_RenderFillRect(this->renderer, &rect);
}

void Sdl_wrapper::put_rect(const glm::ivec2 &position, const glm::ivec2 &dimensions)
{
    SDL_Rect rect = {position.x, position.y, dimensions.x, dimensions.y};
    SDL_RenderDrawRect(this->renderer, &rect);
}

void Sdl_wrapper::put_line(const glm::ivec2 &a, const glm::ivec2 &b)
{
    SDL_RenderDrawLine(this->renderer, a.x, a.y, b.x, b.y);
}

void Sdl_wrapper::present()
{
    SDL_RenderPresent(this->renderer);
}

void Sdl_wrapper::clear()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->renderer);
}
