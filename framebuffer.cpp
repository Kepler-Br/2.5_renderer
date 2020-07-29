#include "framebuffer.h"

int Framebuffer::get_index_from_position(const glm::ivec2 &position)
{
    return position.y * this->resolution.x + position.x;
}

Framebuffer::Framebuffer(Sdl_wrapper &sdl_wrapper):
    sdl_wrapper(sdl_wrapper)
{

}

void Framebuffer::calculate_center()
{
    this->center.x = this->resolution.x/2;
    this->center.y = this->resolution.y/2;
}

void Framebuffer::init(const glm::ivec2 &resolution)
{
    SDL_Renderer *renderer = this->sdl_wrapper.get_renderer();
    if(this->framebuffer != nullptr)
        SDL_DestroyTexture(this->framebuffer);
    this->resolution = resolution;
    this->calculate_center();
    this->pixel_count = resolution.x * resolution.y;
    this->framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, resolution.x, resolution.y);
    if(!this->framebuffer)
        throw std::runtime_error("Cannot create framebuffer texture: " + std::string(SDL_GetError()));
}

void Framebuffer::destroy()
{
    if(this->framebuffer != nullptr)
        SDL_DestroyTexture(this->framebuffer);
    this->framebuffer = nullptr;
}

const glm::ivec2 &Framebuffer::get_resolution() const
{
    return this->resolution;
}

uint32_t *Framebuffer::get_raw() const
{
    return this->pixels;
}

size_t Framebuffer::get_pixel_count() const
{
    return this->pixel_count;
}

const glm::ivec2 &Framebuffer::get_center() const
{
    return this->center;
}

void Framebuffer::lock()
{
    static int pitch;
    if(this->is_locked)
        throw std::runtime_error("Cannot lock buffer twise.");
    this->is_locked = true;
    if(SDL_LockTexture(this->framebuffer, nullptr, (void**)&this->pixels, &pitch))
        throw std::runtime_error("Cannot lock framebuffer: " + std::string(SDL_GetError()));
}

void Framebuffer::unlock()
{
    if(!this->is_locked)
        throw std::runtime_error("Cannot unlcok buffer twise.");
    this->is_locked = false;
    SDL_UnlockTexture(this->framebuffer);

}

void Framebuffer::put()
{
    if(this->is_locked)
        throw std::runtime_error("Cannot put locked buffer.");
    SDL_RenderCopy(this->sdl_wrapper.get_renderer(), this->framebuffer, nullptr, nullptr);
}

void Framebuffer::clear()
{
    if(!this->is_locked)
        throw std::runtime_error("Cannot clear not locked buffer.");
    memset(this->pixels, 0, this->pixel_count*4);
}

void Framebuffer::set_pixel(const Pixel &color, const glm::ivec2 &position)
{
    if(position.x >= this->resolution.x || position.y >= this->resolution.y ||
            position.x < 0 || position.y < 0)
#ifdef DEBUG
        throw std::runtime_error("Set pixel position exceeds framebuffer resolution(Pixel color, ivec2 position).");
#else
        return;
#endif
    const int index = this->get_index_from_position(position);
    ((uint8_t *)(&this->pixels[index]))[0] = color.r;
    ((uint8_t *)(&this->pixels[index]))[1] = color.g;
    ((uint8_t *)(&this->pixels[index]))[2] = color.b;
}

void Framebuffer::set_pixel(const glm::vec3 &color, const glm::ivec2 &position)
{
    if(position.x >= this->resolution.x || position.y >= this->resolution.y ||
            position.x < 0 || position.y < 0)
#ifdef DEBUG
        throw std::runtime_error("Set pixel position exceeds framebuffer resolution(Pixel color, ivec2 position).");
#else
        return;
#endif

    const int index = this->get_index_from_position(position);
    ((uint8_t *)(&this->pixels[index]))[0] = static_cast<uint8_t>(255.0f*color.r);
    ((uint8_t *)(&this->pixels[index]))[1] = static_cast<uint8_t>(255.0f*color.g);
    ((uint8_t *)(&this->pixels[index]))[2] = static_cast<uint8_t>(255.0f*color.b);
}

void Framebuffer::offset_center(const glm::ivec2 &offset)
{
    this->center += offset;
}
