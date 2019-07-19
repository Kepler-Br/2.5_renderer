#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <string>
#include <vector>


class Shader
{
public:
    bool init(const std::string &verticleShader, const std::string &fragmentShader);
    void use();
    void cleanUp();

private:
    bool loadShader(const std::string &fileName, GLenum shaderType);
    // Tries to compile the shader. Returns false if something fails
    bool tryCompileShader(int shaderId);

    // Create shader and set the source
    GLuint createShader(const std::string &fileName, GLenum shaderType);

    bool linkShaders();
    std::string readFile(const std::string &path);

    void printShaderLinkingError(int32_t shaderId);

    // If something went wrong whil compiling the shaders, we'll use this function to find the error
    void printShaderCompilationErrorInfo(int32_t shaderId);

public:
    // The handle to our shader program
    GLuint shaderProgram;
private:

    std::vector<int32_t> shaderIds;

    // The handles to the induvidual shader
    GLuint vertexshader, fragmentShader;
};

#endif // SHADER_H
