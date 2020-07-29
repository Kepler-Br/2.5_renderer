#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/vec2.hpp>
#include <stdexcept>
#include "types.h"
#include "sdl_wrapper.h"

class Framebuffer
{
public:
    glm::ivec2 resolution;
    glm::ivec2 center;
    size_t pixel_count;
    uint32_t *pixels;
    bool is_locked = false;
    SDL_Texture *framebuffer = nullptr;
    Sdl_wrapper &sdl_wrapper;

    int get_index_from_position(const glm::ivec2 &position);

public:
    Framebuffer(Sdl_wrapper &sdl_wrapper);
    /**
     *  \brief Calculate framebuffer center position.
     */
    void calculate_center();
    /**
     *  \brief Initialize SDL framebuffer texture.
     *         If called twise - previous buffer will be replaced.
     */
    void init(const glm::ivec2 &resolution);
    /**
     *  \brief Destroys current SDL buffer.
     */
    void destroy();
    /**
     *  \brief Returns resolution of framebuffer.
     */
    const glm::ivec2 &get_resolution() const;
    /**
     *  \brief Returns pointer to uint32_t pixel array.
     */
    uint32_t *get_raw() const;
    /**
     *  \brief Returns framebuffer width*height.
     */
    size_t get_pixel_count() const;
    /**
     *  \brief Returns framebuffer's center position.
     */
    const glm::ivec2 &get_center() const;
    /**
     *  \brief Locks framebuffer, so we can set framebuffer pixels.
     */
    void lock();
    /**
     *  \brief Unlocks framebuffer, so we can put framebuffer onto window.
     */
    void unlock();
    /**
     *  \brief Put framebuffer onto window.
     */
    void put();
    /**
     *  \brief Clears framebuffer with black color.
     */
    void clear();
    void set_pixel(const Pixel &pixel_color, const glm::ivec2 &position);
    void set_pixel(const glm::vec3 &pixel_color, const glm::ivec2 &position);
    /**
     *  \brief Offset framebuffer's coordinates by value in &offset.
     */
    void offset_center(const glm::ivec2 &offset);
};

#endif // FRAMEBUFFER_H
