#include "playState.h"
#include <iostream>
#include <vector>


PlayState::PlayState(Application *app):
    iGameState(app)
{
    inputManager = InputManager::getInstance();
    sectors = {{0, 5}, {5, 5}, {10, 5}, {15, 5}};
    walls = {{{80.0f, 40.0f}, -1}, {{80.0f, 80.0f}, -1},
             {{200.0f, 80.0f}, 1}, {{200.0f, 40.0f}, -1},
             {{80.0f, 40.0f}, -1},


             {{200.0f, 80.0f}, -1},{{320.0f, 80.0f}, 2},{{320.0f, 40.0f}, -1},{{200.0f, 40.0f}, 0},{{200.0f, 80.0f}, -1},

             {{320.0f, 40.0f}, 2},  {{320.0f, 80.0f}, 3}, {{360.0f, 80.0f}, -1}, {{360.0f, 40.0f}, -1}, {{320.0f, 40.0f}, -1},
               {{320.0f, 80.0f}, -1} ,  {{320.0f, 150.0f}, -1}, {{360.0f, 150.0f}, -1},{{360.0f, 80.0f}, 2},{{320.0f, 80.0f}, -1},
            };
}

PlayState::~PlayState()
{

}

void PlayState::input()
{
    if(inputManager->isEventPending(SDL_QUIT))
        app->exit();
    if(inputManager->isKeyDown(SDLK_ESCAPE))
        app->exit();
    float playerSpeed = 5.0f;
    if(inputManager->isKeyDown(SDLK_w))
        playerPos = glm::vec2(playerPos.x + cos(angle)*playerSpeed, playerPos.y + sin(angle)*playerSpeed);
    if(inputManager->isKeyDown(SDLK_s))
        playerPos = glm::vec2(playerPos.x - cos(angle)*playerSpeed, playerPos.y - sin(angle)*playerSpeed);

    if(inputManager->isKeyDown(SDLK_a))
        angle -= M_PI/20.0f;
    if(inputManager->isKeyDown(SDLK_d))
        angle += M_PI/20.0f;
}

void PlayState::update()
{

}

void PlayState::fixedUpdate()
{
    int prevPlayerSector = lastPlayerSector;
    lastPlayerSector = inside(playerPos, lastPlayerSector);
    if(prevPlayerSector != lastPlayerSector)
        std::cout << "Player sector changed from " << prevPlayerSector << " to " << lastPlayerSector << std::endl;
}

void PlayState::lateUpdate() {}

void PlayState::preRender()
{
    window.setColor(glm::vec3(0.0f));
    window.clear();
}

void PlayState::render()
{
    window.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    const float lookLength = 20.0f;
//    const glm::vec2 point = inputManager->getRelativeMouseCoord();

    window.square(playerPos-glm::vec2(3.0f), glm::vec2(6.0f, 6.0f));
    glm::vec2 playerSight = glm::normalize(playerPos-glm::vec2(playerPos.x + cos(angle)*lookLength, playerPos.y + sin(angle)*lookLength));
    window.line(playerPos, glm::vec2(playerPos.x + cos(angle)*lookLength, playerPos.y + sin(angle)*lookLength));

    for(auto sector: sectors)
    {
        const int numWalls = sector.numWalls;
        const int startWall = sector.startWall;
        for(int i = startWall; i < startWall + numWalls-1; i++)
        {
            const wall currentWall = walls[i];
            const wall nextWall = walls[i+1];

            glm::vec2 wallVector = currentWall.point - nextWall.point;
            glm::vec2 wallNormal = glm::normalize(glm::vec2(-wallVector.y, wallVector.x));
            glm::vec2 normalStartPoint = (currentWall.point + nextWall.point)/2.0f;
            float dot = glm::dot(playerSight, wallNormal);
            const float wallNormalLength = 5.0f;
            window.line(normalStartPoint, normalStartPoint+wallNormal*wallNormalLength);


            if(currentWall.nextSectorIndex == -1)
                window.setColor(glm::vec3(1.0f));
            else
                window.setColor(glm::vec3(1.0f, 0.0f, 0.0f));

            window.line(currentWall.point, nextWall.point);
        }
    }

    std::vector<bunch> bunches = createBunches();

    window.setColor(glm::vec3(1.0f, 0.0f, 1.0f));
    for(const bunch& bnch: bunches)
    {
        for(int wallIndex = bnch.wallIndex; wallIndex < bnch.wallIndex+bnch.wallCount; wallIndex++)
        {
            const wall &curWall = walls[wallIndex];
            const wall &nextWall = walls[wallIndex+1];
            window.line(curWall.point, nextWall.point);
        }
    }


    // Camera space drawing
    {
        glm::vec2 bias(50.0f, 400.0f);

        window.square(bias, glm::vec2(6.0f, 6.0f));
        window.line(bias+3.0f, glm::vec2(bias.x+20.0f+3.0f, bias.y+3.0f));

        for(const bunch& bnch: bunches)
        {
            for(int wallIndex = bnch.wallIndex; wallIndex < bnch.wallIndex+bnch.wallCount; wallIndex++)
            {
                const wall &curWall = walls[wallIndex];
                const wall &nextWall = walls[wallIndex+1];
                glm::vec2 wallEdgeOneWorldPosition = curWall.point-playerPos;
                wallEdgeOneWorldPosition = glm::vec2(wallEdgeOneWorldPosition.x*cos(angle)+wallEdgeOneWorldPosition.y*sin(angle),
                                                     wallEdgeOneWorldPosition.x*sin(angle)-wallEdgeOneWorldPosition.y*cos(angle));
                glm::vec2 wallEdgeTwoWorldPosition = nextWall.point-playerPos;
                wallEdgeTwoWorldPosition = glm::vec2(wallEdgeTwoWorldPosition.x*cos(angle)+wallEdgeTwoWorldPosition.y*sin(angle),
                                                     wallEdgeTwoWorldPosition.x*sin(angle)-wallEdgeTwoWorldPosition.y*cos(angle));
                window.line(wallEdgeOneWorldPosition+bias, wallEdgeTwoWorldPosition+bias);
            }
        }

    }



    // Wall 2.5D drawing.
    for(const bunch& bnch: bunches)
    {
        for(int wallIndex = bnch.wallIndex; wallIndex < bnch.wallIndex+bnch.wallCount; wallIndex++)
        {
            const wall &curWall = walls[wallIndex];
            const wall &nextWall = walls[wallIndex+1];
            glm::vec2 wallEdgeOneWorldPosition = curWall.point-playerPos;
            wallEdgeOneWorldPosition = glm::vec2(wallEdgeOneWorldPosition.x*cos(angle)+wallEdgeOneWorldPosition.y*sin(angle),
                                                 wallEdgeOneWorldPosition.x*sin(angle)-wallEdgeOneWorldPosition.y*cos(angle));
            glm::vec2 wallEdgeTwoWorldPosition = nextWall.point-playerPos;
            wallEdgeTwoWorldPosition = glm::vec2(wallEdgeTwoWorldPosition.x*cos(angle)+wallEdgeTwoWorldPosition.y*sin(angle),
                                                 wallEdgeTwoWorldPosition.x*sin(angle)-wallEdgeTwoWorldPosition.y*cos(angle));
            const float wallHeight = 5.0f;
            window.line(glm::vec2(wallEdgeOneWorldPosition.y, 300.0f-wallHeight/wallEdgeOneWorldPosition.x),
                        glm::vec2(wallEdgeOneWorldPosition.y, 300.0f+wallHeight/wallEdgeOneWorldPosition.x));
        }
    }

}

void PlayState::postRender()
{
    window.rendererPresent();
}
