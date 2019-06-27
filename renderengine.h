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

    void renderMap()
    {
        for(auto &sector : sectors)
        {
            const glm::vec3 bias = glm::vec3(200.0f, 300.0f, 100.0f);
            window.setColor(glm::vec3(1.0f));
            window.square(player.position+bias-glm::vec3(3.0f), glm::vec2(6.0f));
            constexpr float lookLength = 20.0f;
            glm::vec2 playerSight = glm::normalize(player.position-glm::vec3(player.position.x + cos(player.angle)*lookLength,
                                                                             player.position.y + sin(player.angle)*lookLength, 0.0f));
            window.line(player.position+bias, glm::vec3(player.position.x + cos(player.angle)*lookLength, player.position.y + sin(player.angle)*lookLength, 0.0f)+bias);

            const int &startWallIndex = sector.startWall;
            const int &totalWalls = sector.numWalls;
            const Wall startWall = walls[startWallIndex];
            Wall currentWall = startWall;
            for(int wallIndex = startWallIndex; wallIndex < startWallIndex+totalWalls; wallIndex++)
            {
                const glm::vec2 &curPoint = currentWall.point + glm::vec2(bias);
                const int nextPointIndex = currentWall.nextWallIndex;
                const glm::vec2 &nextPoint = walls[nextPointIndex].point + glm::vec2(bias);
                if(currentWall.nextSectorIndex == -1)
                    window.setColor(glm::vec3(1.0f));
                else
                    window.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
                window.line(curPoint, nextPoint);

                currentWall = walls[currentWall.nextWallIndex];
            }
        }
    }

    void render3D()
    {
        glm::vec2 bias = glm::vec2(400.0f, 300.0f);
        Sector currentSector = sectors[player.lastSector];
        Wall currentWall = walls[currentSector.startWall];
        for(int i = 0; i < currentSector.numWalls; i++)
        {
            Wall nextWall = walls[currentWall.nextWallIndex];

            glm::vec2 translateOne = currentWall.point - glm::vec2(player.position);
            glm::vec2 translateTwo = nextWall.point - glm::vec2(player.position);

            glm::vec3 rotateOne = glm::vec3(translateOne.x*player.angleSin-translateOne.y*player.angleCos,
                                            0.0f,
                                            translateOne.x*player.angleCos+translateOne.y*player.angleSin);
            glm::vec3 rotateTwo = glm::vec3(translateTwo.x*player.angleSin-translateTwo.y*player.angleCos,
                                            0.0f,
                                            translateTwo.x*player.angleCos+translateTwo.y*player.angleSin);

            // If wall behind player.
            if(rotateOne.z <= 0.0f && rotateTwo.z  <= 0.0f)
                continue;

            // Clip if partially behind.
            if(rotateOne.z <= 0.0f || rotateTwo.z  <= 0.0f)
            {
                float nearz = 1e-4f;
                float farz = 5.0f;
                float nearside = 1e-5f;
                float farside = 20.0f;

                glm::vec2 i1 = lineIntersection(rotateOne, rotateTwo, glm::vec2(-nearside, nearz), glm::vec2(-farside, farz));
                glm::vec2 i2 = lineIntersection(rotateOne, rotateTwo, glm::vec2(nearside, nearz), glm::vec2(farside, farz));
                if(rotateOne.z < nearz)
                {
                    if(i1.y > 0)
                    {
                        rotateOne.x = i1.x;
                        rotateOne.z = i1.y;
                    }
                    else
                    {
                        rotateOne.x = i2.x;
                        rotateOne.x = i2.y;
                    }
                }

                if(rotateTwo.z < nearz)
                {
                    if(i1.y > 0)
                    {
                        rotateTwo.x = i1.x;
                        rotateTwo.z = i1.y;
                    }
                    else
                    {
                        rotateTwo.x = i2.x;
                        rotateTwo.x = i2.y;
                    }
                }
            }

            constexpr float aspectRatio = 800.0f/600.0f;
            window.line(glm::vec2(rotateOne.x, currentSector.floor/rotateOne.z)*aspectRatio + bias,
                        glm::vec2(rotateTwo.x, currentSector.floor/rotateTwo.z)*aspectRatio + bias);
            window.line(glm::vec2(rotateOne.x, currentSector.ceiling/rotateOne.z)*aspectRatio + bias,
                        glm::vec2(rotateTwo.x, currentSector.ceiling/rotateTwo.z)*aspectRatio + bias);

//            window.line(glm::vec2(rotateOne.x, currentSector.ceiling/rotateOne.z) + bias,
//                        glm::vec2(rotateTwo.x, currentSector.ceiling/rotateTwo.z) + bias);

            currentWall = nextWall;
        }
    }

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
        for(int i = pointOne.x; i < pointTwo.x; i++)
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
        render3D();
//        drawFilledSquare(glm::vec2(0.0f, 100.0f), glm::vec2(100.0f, 100.0f), 10, 20);
    }
};
}

#endif // RENDERENGINE_H
