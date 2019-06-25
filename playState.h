#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "application.h"
#include "inputManager.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>

struct sector
{
    int startWall;
    int numWalls;
    float floor, ceiling;
};

struct wall
{
    glm::vec2 point;
    int nextSectorIndex;
};

struct bunch
{
    int wallIndex;
    int wallCount;
};

struct player
{
    int lastSector;
    glm::vec3 position;
    glm::vec3 velocity;
    float angle,
    angleSin,
    angleCos,
    yaw;
};

class PlayState: public iGameState
{
    InputManager *inputManager;
    std::vector<sector> sectors;
    std::vector<wall> walls;

    player pl;

    void readMap(const std::string &path)
    {
        using namespace std;

        ifstream mapFile(path);
        string line;
        regex sectorRegex("s (\d+) (\d)+")
        while(getline(mapFile, line))
        {

        }
    }

    const std::vector<int> getSectorsToVisit()
    {
        std::vector<int> sectorsToVisit;
        sectorsToVisit.reserve(256);
        sectorsToVisit.push_back(lastPlayerSectorIndex);
        const sector& currentSector = sectors[lastPlayerSectorIndex];

        glm::vec2 playerFacingVector = glm::normalize(playerPos-glm::vec2(playerPos.x + cos(angle), playerPos.y + sin(angle)));
        for(int wallIndex = currentSector.startWall; wallIndex < currentSector.startWall + currentSector.numWalls-1; wallIndex++)
        {
            const wall & currentWall = walls[wallIndex];
            const int sectorPortal = currentWall.nextSectorIndex;
            if(sectorPortal == -1)
                continue;
            const wall & nextWall = walls[wallIndex+1];
            glm::vec2 wallVector = currentWall.point - nextWall.point;
            glm::vec2 wallNormal = glm::normalize(glm::vec2(-wallVector.y, wallVector.x));
            float dot = glm::dot(playerFacingVector, wallNormal);
            if(dot > -0.5f)
            {
                sectorsToVisit.push_back(sectorPortal);
            }
        }

        return sectorsToVisit;
    }

    const std::vector<bunch> createBunches()
    {
        const std::vector<int> sectorsToVisit = getSectorsToVisit();
        std::vector<bunch> bunches;
        bunches.reserve(1024);

        glm::vec2 playerFacingVector = glm::normalize(playerPos-glm::vec2(playerPos.x + cos(angle), playerPos.y + sin(angle)));
        for(const int &secIndex : sectorsToVisit)
        {
            const sector &sec = sectors[secIndex];
            bunch newBunch {-1, -1};
            for(int wallIndex = sec.startWall; wallIndex < sec.startWall+sec.numWalls-1; wallIndex++)
            {
                const wall &currentWall = walls[wallIndex];
                const wall &nextWall = walls[wallIndex+1];
                glm::vec2 wallVector = currentWall.point - nextWall.point;
                glm::vec2 wallNormal = glm::normalize(glm::vec2(-wallVector.y, wallVector.x));
                float dot = glm::dot(playerFacingVector, wallNormal);
                // If wall is visible.
                if(dot > -0.5f)
                {
                    // If newBunch was started.
                    if(newBunch.wallIndex != -1)
                        // Add another wall to it;
                        newBunch.wallCount++;
                    else
                    {
                        newBunch.wallCount = 1;
                        newBunch.wallIndex = wallIndex;
                    }
                }
                else
                    // If new bunch was started but next wall is not visible.
                    if(newBunch.wallIndex != -1)
                    {
                        bunches.push_back(newBunch);
                        newBunch = {-1, -1};
                    }
            }
            // Sector ends. Add bunch to pile if bunch was started;
            if(newBunch.wallIndex != -1)
            {
                bunches.push_back(newBunch);
                newBunch = {-1, -1};
            }
        }
        return bunches;

    }

    void applyLevelCollision(const glm::vec2 &newPlayerPosition)
    {
        auto overlap = [](glm::vec2 one, glm::vec2 two)
        {
            return (glm::min(one.x, one.y) <= glm::max(two.x,two.y) && glm::min(two.x,two.y) <= glm::max(one.x,one.y));
        };
        const sector &playerSector = sectors[lastPlayerSectorIndex];
        for (int wallIndex = playerSector.startWall; wallIndex < playerSector.startWall+ playerSector.numWalls -1; wallIndex++)
        {
            const wall &currentWall = walls[wallIndex];
            const wall &nextWall = walls[wallIndex+1];
            bool isWallIntersects = (overlap(glm::vec2(currentWall.point.x, nextWall.point.x),
                                            glm::vec2(playerPos.x, newPlayerPosition.x)) &&
                                    overlap(glm::vec2(currentWall.point.y, nextWall.point.y),
                                                                     glm::vec2(playerPos.y, newPlayerPosition.y)));
            glm::vec2 intersection;
            if(isWallIntersects)
            {
                if (currentWall.nextSectorIndex != -1)
                {

                    playerPos = newPlayerPosition;
                    return;
                }
                else
                    return;
            }
        }
        playerPos = newPlayerPosition;

    }

    bool isInsideSector(glm::vec2 coord, int sectorIndex)
    {
        const sector sec = sectors[sectorIndex];
        int linesIntersected = 0;
        for(int i = sec.startWall; i < sec.startWall + sec.numWalls-1; i++)
        {
            const glm::vec2 current = walls[i].point;
            const glm::vec2 next = walls[i+1].point;
            if (((current.y>coord.y) != (next.y>coord.y)) &&
                    (coord.x < (next.x-current.x) * (coord.y-current.y) / (next.y-current.y) + current.x) )
                linesIntersected ++;
        }

        return (linesIntersected%2 == 0? false : true);
    }

    int inside(glm::vec2 coord, int lastKnownSector)
    {
        if(lastKnownSector != -1)
        {
            if(isInsideSector(coord, lastKnownSector))
                return lastKnownSector;
            const sector &sec = sectors[lastKnownSector];
            const int startWall = sec.startWall;
            const int numWalls = sec.numWalls;
            for(int i = startWall; i < startWall+numWalls; i++)
            {
                const wall &wll = walls[i];
                const int sectorIndex = wll.nextSectorIndex;
                if(sectorIndex == -1)
                    continue;
                if(isInsideSector(coord, sectorIndex))
                    return sectorIndex;
            }
        }
        for(int i = 0; i < sectors.size(); i++)
        {
            if(i == lastKnownSector)
                continue;
            if(isInsideSector(coord, i))
                return i;
        }
        return -1;
    }


    glm::vec2 intersect(glm::vec2 line11, glm::vec2 line12, glm::vec2 line21, glm::vec2 line22)
    {
        auto cross = [](glm::vec2 one, glm::vec2 two)
        {
            return one.x*two.y-one.y*two.x;
        };

        float x = cross(line11, line12);
        float y = cross(line21, line22);
        float det = cross(glm::vec2(line11.x - line12.x, line11.y - line12.y), glm::vec2(line21.x-line22.x, line21.y-line22.y));
        x = cross(glm::vec2(x, line11.x-line12.x), glm::vec2(y, line21.x-line22.x))/det;
        y = cross(glm::vec2(x, line11.y-line12.y), glm::vec2(y, line21.y-line22.y))/det;
        return glm::vec2(x, y);
    }


public:
    PlayState(Application *app);
    ~PlayState();

    void input();

    void update();
    void fixedUpdate();
    void lateUpdate();

    void preRender();
    void render();
    void postRender();

};

#endif // PLAYSTATE_H
