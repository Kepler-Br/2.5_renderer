#include "renderengine.h"

void xenfa::RenderEngine::renderMap()
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
#include "inputManager.h"

void xenfa::RenderEngine::render3D()
{
    static glm::vec3 pointOne = glm::vec3(10.0f, 0.0f, 2.0f);
    static glm::vec3 pointTwo = glm::vec3(300.0f, 0.0f, 4.0f);
    constexpr float w = 800.0f;
    constexpr float h = 600.0f;
    constexpr float eyeHeigth = 6.0f;
    constexpr float hfov = 0.73f*h;
    constexpr float vfov = 0.2f*h;
    constexpr float ceiling = 20.0f;

    auto &input = *InputManager::getInstance();



    glm::vec2 bias = glm::vec2(300.0f, 400.0f);
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

        window.square(bias, glm::vec2(6.0f));
        window.line(glm::vec2(bias), glm::vec2(bias) + glm::vec2(0.0f, 20.0f));

        if(rotateOne.z <= 0.0f || rotateTwo.z <= 0.0f)
        {
            currentWall = nextWall;
            continue;
        }

        drawFilledSquare(glm::vec2(rotateOne.x, rotateOne.y-ceiling)/rotateOne.z*hfov+bias,
                         glm::vec2(rotateTwo.x, rotateTwo.y-ceiling)/rotateTwo.z*hfov+bias,
                         ceiling/rotateOne.z*hfov,ceiling/rotateTwo.z*hfov);

//        window.line(glm::vec2(rotateOne.x, rotateOne.y-ceiling)/rotateOne.z*hfov+bias,
//        glm::vec2(rotateTwo.x, rotateTwo.y-ceiling)/rotateTwo.z*hfov+bias);

        currentWall = nextWall;
    }

}

void xenfa::RenderEngine::renderRotatedMap()
{
    glm::vec2 bias = glm::vec2(300.0f, 300.0f);
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

        window.square(bias, glm::vec2(6.0f));
        window.line(glm::vec2(bias), glm::vec2(bias) + glm::vec2(0.0f, 20.0f));

        if(rotateOne.z < 0.0f && rotateTwo.z < 0.0f)
        {
            currentWall = nextWall;
            continue;
        }
        if(rotateOne.z < 0.0f || rotateTwo.z < 0.0f)
        {


            if(rotateOne.z < 0.0f)
            {
                glm::vec2 intersection = lineIntersection(glm::vec2(0.0f, 0.0f), glm::vec2(800.0f, 0.0f),
                                                          glm::vec2(rotateTwo.x, rotateTwo.z), glm::vec2(rotateOne.x, rotateOne.z));
                window.line(intersection+bias, glm::vec2(rotateTwo.x, rotateTwo.z)+bias);
            }
            if(rotateTwo.z < 0.0f)
            {

                glm::vec2 intersection = lineIntersection(glm::vec2(0.0f, 0.0f), glm::vec2(800.0f, 0.0f),
                                                          glm::vec2(rotateOne.x, rotateOne.z), glm::vec2(rotateTwo.x, rotateTwo.z));
                window.line(intersection+bias, glm::vec2(rotateOne.x, rotateOne.z)+bias);
            }

        }
        else
        {
             window.line(glm::vec2(rotateOne.x, rotateOne.z)+bias, glm::vec2(rotateTwo.x, rotateTwo.z)+bias);
        }
        currentWall = nextWall;
    }
}
