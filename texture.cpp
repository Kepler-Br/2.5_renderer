//
// Created by kepler-br on 6/10/20.
//

#include "texture.h"
#include <fstream>
#include <iostream>

Texture::Texture(const std::string &path)
{
    this->read(path);
}

Pixel *Texture::get_pixels() const
{
    return this->pixels;
}

void Texture::set_pixel(const Pixel &color, const glm::ivec2 &position)
{
    if (!this->wrap && (position.x >= this->resolution.x || position.y >= this->resolution.y))
        throw std::runtime_error("Tried to set pixel image out of bounds without wrapping set(by ivec2).");
    uint index = position.y * this->resolution.x + position.x;
    this->pixels[index] = color;
}

void Texture::free()
{
    delete []this->pixels;
}

const size_t &Texture::get_pixel_count() const
{
    return this->pixel_count;
}

const glm::ivec2 &Texture::get_resolution() const
{
    return this->resolution;
}

bool Texture::is_transparent() const
{
    return transparent;
}

const Pixel &Texture::get_pixel(const uint &index) const
{
    if(index > this->pixel_count)
        throw std::runtime_error("Tried to access pixel image out of bounds without wrapping set(by int).");
    return this->pixels[index];
}

const Pixel &Texture::get_pixel(const glm::ivec2 &position) const
{
    static glm::ivec2 wrapped_position;
    static uint index;
    if (!this->wrap && (position.x >= this->resolution.x || position.y >= this->resolution.y))
        throw std::runtime_error("Tried to access pixel image out of bounds without wrapping set(by ivec2).");
    else if(this->wrap)
    {
        wrapped_position.x = std::abs(position.x%this->resolution.x);
        wrapped_position.y = std::abs(position.y%this->resolution.y);
        index = wrapped_position.y * this->resolution.x + wrapped_position.x;
    }
    else
        index = position.y * this->resolution.x + position.x;
    return this->pixels[index];
}

const Pixel &Texture::get_normalized_pixel(const glm::vec2 &position) const
{
    glm::ivec2 scaled_position = glm::ivec2(position.x * this->resolution.x, position.y * this->resolution.y);
//    if(this->wrap)
    {
//        scaled_position.x = scaled_position.x % this->resolution.x;
//        scaled_position.y = scaled_position.y % this->resolution.y;
    }
    return this->get_pixel(scaled_position);
}


void Texture::set_wrapping(const bool &wrap)
{
    this->wrap = wrap;
}

void Texture::read(const std::string &path)
{
    uint8_t file_sign[3];
    std::ifstream file(path);
    if(file.bad() || !file.is_open())
        throw std::runtime_error("Cannot open image: " + path);
    file.read((char *) file_sign, 3);
    if (file_sign[0] != 'I' && file_sign[1] != 'M' && file_sign[2] != 'G')
        throw std::runtime_error(path + " is not a proper image file.");

    static uint16_t resolution[2];
    file.read((char *) resolution, sizeof(uint16_t) * 2);
    this->resolution = glm::ivec2(resolution[0], resolution[1]);
    this->pixel_count = this->resolution.x * this->resolution.y;
    delete []this->pixels;
    this->pixels = new Pixel[pixel_count];
    file.read((char*)this->pixels, sizeof(Pixel)*this->pixel_count);
    if(file.gcount() != (std::streamsize)(sizeof(Pixel)*this->pixel_count))
        throw std::runtime_error("Abrupt end of the texture: " + path);
}
