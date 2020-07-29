//
// Created by kepler-br on 6/10/20.
//

#ifndef WOLFENSHETIN_TEXTURE_H
#define WOLFENSHETIN_TEXTURE_H

#include <string>
#include <glm/vec2.hpp>
#include "types.h"

class Texture_loader;

class Texture
{
private:
    friend Texture_loader;

    bool transparent;
    bool wrap = true;
    Pixel *pixels = nullptr;
    glm::ivec2 resolution;
    size_t pixel_count;

    void read_file(const std::string &path);

public:
    Texture(const std::string &path);
    Texture() = default;

    void free();
    Pixel *get_pixels() const;
    void set_pixel(const Pixel &color, const glm::ivec2 &position);
    const Pixel &get_pixel(const uint &index) const;
    const Pixel &get_pixel(const glm::ivec2 &position) const;
    const Pixel &get_normalized_pixel(const glm::vec2 &position) const;
    const size_t &get_pixel_count() const;
    const glm::ivec2 &get_resolution() const;
    bool is_transparent() const;
    void set_wrapping(const bool &wrap);
    void read(const std::string &path);
};


#endif //WOLFENSHETIN_TEXTURE_H
