#include "shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

bool Shader::init(const std::string &verticleShader, const std::string &fragmentShader)
{
    // Generate our shader. This is similar to glGenBuffers() and glGenVertexArray()
    // except that this returns the ID
    shaderProgram = glCreateProgram();

    if (!loadShader(verticleShader, GL_VERTEX_SHADER))
        return false;

    if (!loadShader(fragmentShader, GL_FRAGMENT_SHADER))
        return false;

    // All shaders has been create, now we must put them together into one large object
    return linkShaders();
}

bool Shader::loadShader(const std::string &fileName, GLenum shaderType)
{
    std::cout << "Loading Shader : " << fileName << std::endl;

    GLuint shaderId = createShader(fileName, shaderType);

    if (tryCompileShader(shaderId))
    {
        glAttachShader(shaderProgram, shaderId);
        shaderIds.push_back(shaderId);

        return true;
    }

    return false;
}

void Shader::use()
{
    // Load the shader into the rendering pipeline
    glUseProgram(shaderProgram);
}

void Shader::cleanUp()
{
    // Cleanup all the things we bound and allocated
    glUseProgram(0);

    for ( auto i : shaderIds)
        glDetachShader(shaderProgram, i);

    glDeleteProgram(shaderProgram);

    for ( auto i : shaderIds)
        glDeleteShader(i);
}

bool Shader::tryCompileShader(int shaderId)
{
    // Compile the vertex shader
    glCompileShader(shaderId);

    // Ask OpenGL if the shaders was compiled
    int wasCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &wasCompiled);

    printShaderCompilationErrorInfo(shaderId);

    // Return false if compilation failed
    return (wasCompiled != 0);
}

GLuint Shader::createShader(const std::string &fileName, GLenum shaderType)
{
    // Read file as std::string
    std::string str = readFile(fileName.c_str());

    // c_str() gives us a const char*, but we need a non-const one
    char* src = const_cast<char*>( str.c_str());
    int32_t size = str.length();

    // Create an empty vertex shader handle
    int shaderId = glCreateShader(shaderType);

    // Send the vertex shader source code to OpenGL
    glShaderSource(shaderId , 1, &src, &size);

    return shaderId;
}

bool Shader::linkShaders()
{
    // At this point, our shaders will be inspected/optized and the binary code generated
    // The binary code will then be uploaded to the GPU
    glLinkProgram(shaderProgram);

    // Verify that the linking succeeded
    int isLinked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

    if (isLinked == false)
        printShaderLinkingError(shaderProgram);

    return isLinked != 0;
}

std::string Shader::readFile(const std::string &path)
{
    // Open file
    std::ifstream file(path);

    // Read file into buffer
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Make a std::string and fill it with the contents of buffer
    std::string fileContent = buffer.str();

    return fileContent;
}

void Shader::printShaderLinkingError(int32_t shaderId)
{
    std::cout << "=======================================\n";
    std::cout << "Shader linking failed : " << std::endl;

    // Find length of shader info log
    int maxLength;
    glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Info Length : " << maxLength << std::endl;

    // Get shader info log
    char* shaderProgramInfoLog = new char[maxLength];
    glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

    std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

    // Handle the error by printing it to the console
    delete shaderProgramInfoLog;
    return;
}

void Shader::printShaderCompilationErrorInfo(int32_t shaderId)
{
    // Find length of shader info log
    int maxLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

    // Get shader info log
    char* shaderInfoLog = new char[maxLength];
    glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

    std::string log = shaderInfoLog;

    constexpr int minLength = 5;
    if (log.length() > minLength)
    {
        std::cout << "=======================================\n";
        std::cout <<  shaderInfoLog << std::endl;
        std::cout << "=======================================\n\n";
    }

    // Print shader info log
    delete shaderInfoLog;
}
