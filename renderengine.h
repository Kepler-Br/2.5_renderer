#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

#include "window.h"
#include "types.h"
#include "camera.h"

#include "shader.h"
#include "texturecontainer.h"

namespace xenfa
{
class RenderEngine
{

    struct MeshGL
    {
        GLuint vertexArray;
        GLuint vertexBuffer;
        GLuint numVertices;
        GLuint uvBuffer;
    };


    std::vector<MeshGL> wallMeshes;
    std::vector<MeshGL> ceilingMeshes;
    std::vector<MeshGL> floorMeshes;
    TextureContainer textureContainer;

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
    void init(const std::vector<std::pair<uint, std::string>> &texturePaths)
    {
        camera.lookAt(glm::vec3(40.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        camera.projectionPerspective(M_PI/2.0, 800.0f/600.0f, 0.001f, 200.0f);
        generateWallsVertexBuffers();
        generateSectorVertexBuffers();
        shader.init("vert.glsl", "frag.glsl");
        for(const auto &texturePath: texturePaths)
        {
            uint index;
            std::string path;
            std::tie(index, path) = texturePath;
            textureContainer.loadTexture(index, path);
        }

    }

    void render();
};
}

#endif // RENDERENGINE_H
