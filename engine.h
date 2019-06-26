#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>

#include "window.h"
#include "inputManager.h"


class Engine
{
    struct Player
    {
        int lastSector;
        glm::vec3 position;
        glm::vec3 velocity;
        float angle,
        angleSin,
        angleCos,
        yaw;
    };

    struct Wall
    {
        glm::vec2 point;
        int nextWallIndex;
        int nextSectorIndex;
    };

    struct Sector
    {
        int startWall;
        int numWalls;
        float floor, ceiling;
    };

    struct Bunch
    {
        int wallIndex;
        int wallCount;
    };


    std::vector<Wall> walls;
    std::vector<Sector> sectors;

    Player player;
    Window& window;
    InputManager &inputManager;


    bool overlap(const glm::vec2 &one, const glm::vec2 &two)
    {
        return (glm::min(one.x, one.y) <= glm::max(two.x,two.y) && glm::min(two.x,two.y) <= glm::max(one.x,one.y));
    }

    bool isLinesIntersected(const glm::vec2 &line11, const glm::vec2 &line12, const glm::vec2 &line21, const glm::vec2 &line22)
    {
        return (overlap(glm::vec2(line11.x, line21.y),
                        glm::vec2(line12.x, line22.x)) &&
                overlap(glm::vec2(line11.y, line21.y),
                        glm::vec2(line12.y, line22.y)));
    }

    bool isInsideSector(glm::vec3 coord, int sectorIndex)
    {
        const Sector sec = sectors[sectorIndex];
        int linesIntersected = 0;
        for(int i = sec.startWall; i < sec.startWall + sec.numWalls-1; i++)
        {
            const glm::vec2 current = walls[i].point;
            const glm::vec2 next = walls[i+1].point;
            if (((current.y>coord.y) != (next.y>coord.y)) &&
                    (coord.x < (next.x-current.x) * (coord.y-current.y) / (next.y-current.y) + current.x) )
                linesIntersected ++;
        }
        bool insideXYSectorPlane = (linesIntersected%2 == 0? false : true);
        bool insideZSectorPlane = coord.z > sec.floor && coord.z < sec.ceiling;

        return insideXYSectorPlane & insideZSectorPlane;
    }

    int checkObjectSector(const glm::vec3 &position, const int lastKnownSectorIndex);
    int getCrossedWall(const int sectorIndex, const glm::vec3 &oldPosition, const glm::vec3 &newPosition)
    {
        const Sector &sector = sectors[sectorIndex];
        int curWallIndex = sector.startWall;
        Wall curWall = walls[curWallIndex];
        for(int i = 0; i < sector.numWalls; i++)
        {
            Wall &nextWall = walls[curWall.nextWallIndex];
            if(isLinesIntersected(curWall.point, nextWall.point, oldPosition, newPosition))
            {
                return curWallIndex;
            }
            curWallIndex = curWall.nextWallIndex;
            curWall = nextWall;
        }
    }

public:
    Engine(Window &window):
        window(window),
        inputManager(*InputManager::getInstance())
    {
    }

    void fixedUpdate();

    void input();

    void levelCollision(const glm::vec3 &newPosition);

    void render();
    void readMap(const std::string& path);


};

#endif // ENGINE_H
