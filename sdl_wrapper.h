//
// Created by kepler-br on 6/2/20.
//

#ifndef WOLFENSHETIN_SDL_WRAPPER_H
#define WOLFENSHETIN_SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "types.h"

class Sdl_wrapper
{
private:
    SDL_Window *window = nullptr;
    SDL_Renderer* renderer = nullptr;
    glm::ivec2 resolution;

    void init_sdl();

public:

    int pixel_count;
    Sdl_wrapper() = default;

    explicit Sdl_wrapper(const glm::ivec2 &resolution);
    SDL_Window *get_window() const;
    SDL_Renderer *get_renderer() const;
    const glm::ivec2 &get_resolution() const;
    // Direct SDL2 operations.
    void set_color(const glm::ivec3 &color);
    void set_color(const uint8_t &red, const uint8_t &green, const uint8_t &blue);
    void set_pixel(const glm::ivec2 &position);
    void put_filled_rect(const glm::ivec2 &position, const glm::ivec2 &dimensions);
    void put_rect(const glm::ivec2 &position, const glm::ivec2 &dimensions);
    void put_line(const glm::ivec2 &a, const glm::ivec2 &b);
    void present();
    void clear();
};


#endif //WOLFENSHETIN_SDL_WRAPPER_H
