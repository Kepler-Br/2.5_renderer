#ifndef WINDOW_H
#define WINDOW_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>

class Window
{
private:
    glm::ivec2 geometry;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    /**
     *  \brief Initializes SDL2 renderer and window.
     *
     *  \param geometry geometry of the window.
     */
    void init(const glm::ivec2 &geometry);

public:
    Window(const glm::ivec2 &geometry, const std::string &windowTitle = "SDL2 window");
    Window(const int x, const int y, const std::string &windowTitle = "SDL2 window");
    ~Window();

    /**
     *  \brief Sets window title.
     *
     *  \param title String to which set window title.
     */
    void setWindowTitle(const std::string &title);

    /**
     *  \brief Sets window position on screen.
     *
     *  \param position Integer vector to where move the window.
     */
    void setWindowPosition(const glm::ivec2 &position);

    /**
     *  \brief Returns geometry of the SDL2 window.
     *
     *  \return width and height of the window in int.
     */
    const glm::ivec2 getGeometry() const;


    void setColor(const glm::vec3 &color)
    {
        SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(color.r*255),
                               static_cast<Uint8>(color.g*255),
                               static_cast<Uint8>(color.b*255),
                               255);
    }

    void setColor(const glm::ivec3 &color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b*255, 255);
    }
    /**
     *  \brief Sets pixel at position to color.
     *
     *  \param position coordinates of the point.
     *  \param color value from 0.0f to 1.0f.
     */
    void setPixel(const glm::ivec2 &position);
    /**
     *  \brief Clears the renderer.
     *
     *  \param color Color value from 0.0f to 1.0f.
     */
    void clear();

    /**
     *  \brief Updates renderer with new pixels.
     */
    void rendererPresent()
    {
        SDL_RenderPresent(renderer);
    }

    void line(glm::vec2 point1, glm::vec2 point2)
    {
        SDL_RenderDrawLine(renderer, static_cast<int>(point1.x), static_cast<int>(point1.y),
                                     static_cast<int>(point2.x), static_cast<int>(point2.y));
    }

    void vLine(glm::vec2 bottom, float length)
    {
        const glm::vec2 top = glm::vec2(bottom.x, bottom.y+length);
        SDL_RenderDrawLine(renderer, static_cast<int>(bottom.x), static_cast<int>(bottom.y),
                                     static_cast<int>(top.x), static_cast<int>(top.y));
    }

    void square(const glm::vec2 &position, const glm::vec2 &geometry)
    {
        SDL_Rect rect = {static_cast<int>(position.x),
                         static_cast<int>(position.y),
                         static_cast<int>(geometry.x),
                         static_cast<int>(geometry.y)};
        SDL_RenderFillRect(renderer,
                           &rect);
    }

//    void fillRectangle(glm::vec2 )
};

#endif // WINDOW_H
