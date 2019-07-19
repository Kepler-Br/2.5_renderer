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
    SDL_GLContext glContext = nullptr;

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

    /**
     *  \brief Clears the renderer.
     *
     *  \param color Color value from 0.0f to 1.0f.
     */
    void clear(const glm::vec3 &color);

    /**
     *  \brief Clears the renderer with black color.
     */
    void clear();

    /**
     *  \brief Swaps opengl buffer.
     */
    void swapBuffer();

    /**
     *  \brief Prints to stdio major and minor version of OpenGL.
     */
    void printOpenGLVersion();
};

#endif // WINDOW_H
