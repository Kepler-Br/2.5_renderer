#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <stdexcept>
#include <map>

#include <GL/gl.h>
#include <SDL2/SDL.h>

namespace xenfa
{
class TextureContainer
{
    GLuint readAndUploadTexture(const std::string &path);
    std::map<uint, GLuint> textures;

public:
    TextureContainer();

    void loadTexture(const uint &index, const std::string &path);
    GLuint getTexture(const uint &index) const;
};
}

#endif // TEXTURELOADER_H
