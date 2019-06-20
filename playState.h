#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "application.h"
#include "inputManager.h"
#include <vector>

struct sector
{
    int startWall;
    int numWalls;
};

struct wall
{
    glm::vec2 point;
    int nextSectorIndex;
};

class PlayState: public iGameState
{
    InputManager *inputManager;
    std::vector<sector> sectors;
    std::vector<wall> walls;

    glm::vec2 point1 = glm::vec2(70.0f, 10.0f);
    glm::vec2 point2 = glm::vec2(70.0f, 170.0f);
    glm::vec2 playerPos = glm::vec2(50.0f);
     float angle = 0.0f;

    bool insideSector(glm::vec2 coord, int sectorIndex)
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

    bool intersect(glm::vec3 line11, glm::vec3 line12, glm::vec2 line21, glm::vec2 line22)
    {
        glm::vec3 x = glm::cross(line11, line12);


    }
};

#endif // PLAYSTATE_H
