#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <vector>

#include "window.h"
#include "types.h"

namespace xenfa
{
class RenderEngine
{
    std::vector<Wall> &walls;
    std::vector<Sector> &sectors;
    Player &player;
    Window &window;

    glm::vec2 lineIntersection(glm::vec2 point11, glm::vec2 point12, glm::vec2 point21, glm::vec2 point22)
    {

        float x = glm::determinant(glm::mat2(point11, point12));
        float y = glm::determinant(glm::mat2(point21, point22));
        float det = glm::determinant(glm::mat2(glm::vec2(point11.x - point12.x, point11.y - point12.y),
                                               glm::vec2(point21.x-point22.x, point21.y-point22.y)));
        x = glm::determinant(glm::mat2(glm::vec2(x, point11.x-point12.x), glm::vec2(y, point21.x-point22.x)))/det;
        y = glm::determinant(glm::mat2(glm::vec2(x, point11.y-point12.y), glm::vec2(y, point21.y-point22.y)))/det;
        return glm::vec2(x, y);
    }

    void renderMap();

    void render3D();
    void renderRotatedMap();

    float lerp(float v0, float v1, float t)
    {
        return v0 + t * (v1 - v0);
    }

    void drawFilledSquare(const glm::vec2 &pointOne, const glm::vec2 pointTwo, const float heightOne, const float heightTwo)
    {
        auto interpolation = [](glm::vec2 one, glm::vec2 two, float x)
        {
            return one.y+(x-one.x)*(two.y-one.y)/(two.x-one.x);
        };
        for(int i = glm::min(pointOne.x, pointTwo.x); i < glm::max(pointOne.x, pointTwo.x); i++)
        {
            float y = interpolation(pointOne, pointTwo, i);
            window.vLine(glm::vec2(i, y), lerp(heightOne, heightTwo, i/pointTwo.x));
        }
    }

public:
    RenderEngine(std::vector<Wall> &walls,
                 std::vector<Sector> &sectors,
                 Player& player,
                 Window &window):
        walls(walls),
        sectors(sectors),
        player(player),
        window(window)
    {}

    void render()
    {
        renderMap();
//        renderRotatedMap();
        render3D();
        //        drawFilledSquare(glm::vec2(0.0f, 100.0f), glm::vec2(100.0f, 100.0f), 10, 20);
    }
};
}

#endif // RENDERENGINE_H
