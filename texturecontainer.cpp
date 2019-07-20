#include "texturecontainer.h"


GLuint xenfa::TextureContainer::readAndUploadTexture(const std::string &path)
{
    glActiveTexture(GL_TEXTURE0);
    SDL_Surface* textureImage;
    textureImage = SDL_LoadBMP(path.c_str());
    if(!textureImage)
    {
        throw std::runtime_error("Error loading texture: " + path);
    }

    GLuint texture;
    glGenTextures(1 , &texture);
    glBindTexture(GL_TEXTURE_2D , texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->w ,
                 textureImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels);
    //Free up the memory.
    if(textureImage)
        SDL_FreeSurface(textureImage);

    return texture;

}

xenfa::TextureContainer::TextureContainer()
{}

void xenfa::TextureContainer::loadTexture(const uint &index, const std::string &path)
{
    GLuint textureGLIndex = readAndUploadTexture(path);
    textures[index] = textureGLIndex;
}

GLuint xenfa::TextureContainer::getTexture(const uint &index) const
{
    if(textures.count(index) == 0)
        throw std::runtime_error("Cannot get texture under index " + std::to_string(index));
    return textures.at(index);
}
