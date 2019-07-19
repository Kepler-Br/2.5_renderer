#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "window.h"
#include "types.h"
#include "camera.h"

#include "shader.h"

namespace xenfa
{
class RenderEngine
{

    struct WallGL
    {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint numVertices;
        GLuint uvBuffer;
    };

    struct SectorGL
    {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint numVertices;
        GLuint uvBuffer;
    };


    std::vector<WallGL> wallsGL;
    std::vector<SectorGL> sectorCeilingsGL;
    std::vector<SectorGL> sectorFloorsGL;

    std::vector<Wall> &walls;
    std::vector<Sector> &sectors;
    Player &player;
    Window &window;

    Camera camera;

//    glm::vec2 lineIntersection(glm::vec2 point11, glm::vec2 point12, glm::vec2 point21, glm::vec2 point22)
//    {

//        float x = glm::determinant(glm::mat2(point11, point12));
//        float y = glm::determinant(glm::mat2(point21, point22));
//        float det = glm::determinant(glm::mat2(glm::vec2(point11.x - point12.x, point11.y - point12.y),
//                                               glm::vec2(point21.x-point22.x, point21.y-point22.y)));
//        x = glm::determinant(glm::mat2(glm::vec2(x, point11.x-point12.x), glm::vec2(y, point21.x-point22.x)))/det;
//        y = glm::determinant(glm::mat2(glm::vec2(x, point11.y-point12.y), glm::vec2(y, point21.y-point22.y)))/det;
//        return glm::vec2(x, y);
//    }

    std::vector<float> generatePortalVertices(uint wallIndex, uint sectorIndex);
    std::vector<float> generateWallVertices(int wallIndex, int sectorIndex);

    std::vector<float> generatePortalUV(uint wallIndex, uint sectorIndex);
    std::vector<float> generateWallUV(int wallIndex, int sectorIndex);


    std::vector<float> generateSectorVertices(uint sectorIndex, bool generateFloor);
    std::vector<float> generateSectorUV(uint sectorIndex);

    void setupVertexObjects(GLuint &Vao, GLuint &Vbo, GLuint &VboUV,
                            const std::vector<float> &vertices,
                            const std::vector<float> &UV);

    void generateWallsVertexBuffers();

    void generateSectorVertexBuffers();

public:
    Shader shader;

    RenderEngine(std::vector<Wall> &walls,
                 std::vector<Sector> &sectors,
                 Player& player,
                 Window &window):
        walls(walls),
        sectors(sectors),
        player(player),
        window(window)
    {

    }
    GLuint texture;
    void init()
    {
        camera.lookAt(glm::vec3(40.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        camera.projectionPerspective(M_PI/2.0, 800.0f/600.0f, 0.001f, 200.0f);
        generateWallsVertexBuffers();
        generateSectorVertexBuffers();
        shader.init("vert.glsl", "frag.glsl");
        texture = loadGLTextures("wall.bmp");

    }
    GLuint loadGLTextures(const std::string &path)
    {
        glActiveTexture(GL_TEXTURE0);
        GLuint texture;
        SDL_Surface* textureImage;
        textureImage = SDL_LoadBMP(path.c_str());
        if(!textureImage)
        {
            throw std::runtime_error("Error loading texture");
        }

        //Create the texture.
        glGenTextures(1 , &texture);

        //Typical texture generation using data from the bitmap.
        glBindTexture(GL_TEXTURE_2D , texture);

        //Generate the texture.
        glTexImage2D(GL_TEXTURE_2D , 0 , 3 , textureImage->w ,
                     textureImage->h , 0 , GL_RGB , GL_UNSIGNED_BYTE ,
                     textureImage->pixels);

        //Linear filtering.
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);

        //Free up the memory.
        if(textureImage)
            SDL_FreeSurface(textureImage);

        return texture;

    }

    void render();
};
}

#endif // RENDERENGINE_H
