#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <vector>
#include <fstream>
#include <iostream>
#include <regex>

#include "application.h"
#include "inputManager.h"
#include "physicsengine.h"
#include "renderengine.h"


namespace xenfa
{
class PlayState: public iGameState
{

    std::vector<Wall> walls;
    std::vector<Sector> sectors;

    InputManager &inputManager;
    PhysicsEngine physicsEngine;
    RenderEngine renderEngine;

    Player player;

    void readMap(const std::string& path);


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
}

#endif // PLAYSTATE_H
