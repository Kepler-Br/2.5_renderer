#include "playState.h"
#include <iostream>
#include <vector>


PlayState::PlayState(Application *app):
    iGameState(app)
{
    inputManager = InputManager::getInstance();
    sectors = {{0, 5}, {5, 5}};
    walls = {{{80.0f, 40.0f}, -1}, {{80.0f, 80.0f}, -1},
             {{200.0f, 80.0f}, 1}, {{200.0f, 40.0f}, -1},
             {{80.0f, 40.0f}, -1},

             {{200.0f, 80.0f}, 0}, {{200.0f, 40.0f}, -1},
              {{320.0f, 40.0f}, -1},
             {{320.0f, 80.0f}, -1}, {{200.0f, 80.0f}, -1}};
}

PlayState::~PlayState()
{

}

void PlayState::input()
{

}

void PlayState::update()
{

}

void PlayState::fixedUpdate()
{
    if(inputManager->isKeyDown(SDLK_ESCAPE))
        app->exit();
    if(inputManager->isKeyDown(SDLK_w))
        playerPos = glm::vec2(playerPos.x + cos(angle), playerPos.y + sin(angle));
    if(inputManager->isKeyDown(SDLK_s))
        playerPos = glm::vec2(playerPos.x - cos(angle), playerPos.y - sin(angle));

    if(inputManager->isKeyDown(SDLK_a))
        angle -= M_PI/20.0f;
    if(inputManager->isKeyDown(SDLK_d))
        angle += M_PI/20.0f;

//    #define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))

}

void PlayState::lateUpdate() {}

void PlayState::preRender()
{
    window.setColor(glm::vec3(0.0f));
    window.clear();
}

void PlayState::render()
{
//    window.setColor(glm::vec3(1.0f));
//    const float lookLength = 20.0f;
//    const float bias = 50.0f;
//    std::vector<std::pair<glm::vec3, glm::vec3>> level = {{{70.0f, 0.0f, 10.0f}, {70.0f, 0.0f, 170.0f}},
//                                                          {{70.0f, 0.0f, 170.0f}, {10.0f, 0.0f, 170.0f}},};

//    window.line(glm::vec2(0.0f)+bias, glm::vec2(lookLength, 0.0f)+bias);
//    window.square(glm::vec2(-2, -2)+bias, glm::vec2(4, 4));
//    for(auto& pair : level)
//    {
//        glm::vec3 p1;
//        glm::vec3 p2;
//        std::tie(p1, p2) = pair;
//        glm::vec3 t1(p1.x-playerPos.x, 0.0f, p1.z-playerPos.y);
//        glm::vec3 t2(p2.x-playerPos.x, 0.0f, p2.z-playerPos.y);
//        //        glm::mat3 rotMat = {  1,          0,           0,
//        //                              0, cos(angle), -sin(angle),
//        //                              0, sin(angle),  cos(angle),
//        //                           };
//        //                glm::mat3 rotMat = { cos(angle), -sin(angle), 0,
//        //                                     sin(angle),  cos(angle), 0,
//        //                                     0,          0,           1};
//        glm::mat3 rotMat(
//                    cos(angle), 0, sin(angle),
//                    0,         1,           0,
//                    -sin(angle), 0, cos(angle)
//                    );
//        rotMat = glm::transpose(rotMat);
//        t1 = rotMat*t1;
//        t2 = rotMat*t2;
//        window.line(glm::vec2(t1.x, t1.z)+bias, glm::vec2(t2.x, t2.z)+bias);

//        float x1 = t1.z*106/t1.x;
//        float x2 = t2.z*106/t2.x;
//        if(t1.x > 0 or t2.x > 0)
//        {
//            window.line(glm::vec2(x1, -50)+400.0f, glm::vec2(x2, -50)+400.0f);
//            window.line(glm::vec2(x1, -60)+400.0f, glm::vec2(x2, -60)+400.0f);
//            window.line(glm::vec2(x1, -60)+400.0f, glm::vec2(x1, -50)+400.0f);
//            window.line(glm::vec2(x2, -60)+400.0f, glm::vec2(x2, -50)+400.0f);
//        }

//    }
    window.setColor(glm::vec3(1.0f, 1.0f, 1.0f));

    const glm::vec2 point = inputManager->getRelativeMouseCoord();

    window.square(point, glm::vec2(6.0f, 6.0f));

    for(auto sector: sectors)
    {
        const int numWalls = sector.numWalls;
        const int startWall = sector.startWall;
        for(int i = startWall; i < startWall + numWalls-1; i++)
        {
            const wall currentWall = walls[i];
            const wall nextWall = walls[i+1];
            if(currentWall.nextSectorIndex == -1)
                window.setColor(glm::vec3(1.0f));
            else
                window.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
            window.line(currentWall.point, nextWall.point);
        }
    }

    for (int i = 0; i < sectors.size(); i++)
    {
        if(insideSector(point, i))
        {
            std::cout << "Player is inside " << i << " sector." << std::endl;
            break;
        }
    }
//    for(int currentPointIndex = 0; currentPointIndex < polygon.size(); currentPointIndex++)
//    {
//        if(currentPointIndex == 0)
//            continue;
//        const glm::vec2 &previous = polygon[currentPointIndex-1];
//        const glm::vec2 &current = polygon[currentPointIndex];
//        window.line(previous, current);
//        if ( ((current.y>point.y) != (previous.y>point.y)) &&
//             (point.x < (previous.x-current.x) * (point.y-current.y) / (previous.y-current.y) + current.x) )
//            linesIntersected ++;

//    }

//    std::cout << (linesIntersected%2 == 0? "outside" : "inside") << std::endl;

}

void PlayState::postRender()
{
    window.rendererPresent();
}
