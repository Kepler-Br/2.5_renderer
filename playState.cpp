#include "playState.h"
#include <iostream>
#include <vector>
#include <GL/gl.h>

using namespace xenfa;

PlayState::PlayState(Application *app):
    iGameState(app),
    inputManager(*InputManager::getInstance()),
    physicsEngine(walls, sectors),
    renderEngine(walls, sectors, player, window)
{
    readMap("map.txt");
    renderEngine.init();
}

PlayState::~PlayState()
{

}

void PlayState::input()
{
    if(inputManager.isEventPending(SDL_QUIT))
        app->exit();
    if(inputManager.isKeyDown(SDLK_ESCAPE))
        app->exit();

    constexpr float playerSpeed = 5.0f;
    glm::vec3 playerMovement(0.0f);
    if(inputManager.isKeyDown(SDLK_w))
    {
        glm::vec3 newPlayerPosition = glm::vec3(cos(player.angle),
                                                sin(player.angle),
                                                0.0f);
        playerMovement = newPlayerPosition;
    }
    if(inputManager.isKeyDown(SDLK_s))
    {
        glm::vec3 newPlayerPosition = glm::vec3(- cos(player.angle),
                                                - sin(player.angle),
                                                0.0f);
        playerMovement = newPlayerPosition;

    }
    player.velocity = playerMovement*playerSpeed;
    int lastSector = player.lastSector;
    player.lastSector = physicsEngine.checkObjectSector(player.position, player.lastSector);
    if(lastSector != player.lastSector)
        std::cout << "sector changed to " << player.lastSector << std::endl;


    if(inputManager.isKeyDown(SDLK_RIGHT))
        player.angle -= M_PI/20.0f;
    if(inputManager.isKeyDown(SDLK_LEFT))
        player.angle += M_PI/20.0f;
    if(inputManager.isKeyDown(SDLK_UP))
        player.yaw -= M_PI/20.0f;
    if(inputManager.isKeyDown(SDLK_DOWN))
        player.yaw += M_PI/20.0f;


}

void PlayState::update()
{

}

void PlayState::fixedUpdate()
{
    player.position = physicsEngine.levelCollision(player.position, player.position + player.velocity, player.lastSector);
    player.angleCos = cos(player.angle);
    player.angleSin = sin(player.angle);
    player.yawCos = cos(player.yaw);
    player.yawSin = sin(player.yaw);
}

void PlayState::lateUpdate()
{

}

void PlayState::preRender()
{
//    window.setColor(glm::vec3(0.0f));
    window.clear();

}

void PlayState::render()
{
    renderEngine.render();
}

void PlayState::postRender()
{
    window.swapBuffer();
}

void PlayState::readMap(const std::string &path)
{
    using namespace std;

    ifstream mapFile;
    mapFile.open(path);
    string line = "1";
    regex pstartRegex("^ *pstart +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(\\d+) *$");
    regex sectorRegex("^ *s +(\\d+) +(\\d+) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(\\d+) +(\\d+) *$");
    regex wallRegex(" *w +(\\-?\\d+\\.?\\d*) +(\\-?\\d+\\.?\\d*) +(\\d+) +(\\-?\\d+) +(\\d+) +(\\-?\\d+) +(\\-?\\d+) +(\\-?\\d+) +(\\-?\\d+) *");


    regex secnumRegex("^ *secnum +(\\d+) *$");
    regex wallnumRegex("^ *wallnum +(\\d+) *$");

    while(getline(mapFile, line))
    {
        std::cmatch m;
        if(mapFile.eof())
            break;
        if(regex_match(line.c_str(), m, wallRegex))
        {
            Wall wall;
            wall.point = glm::vec2(stof(m[1]), stof(m[2]));
            wall.nextWallIndex = stoi(m[3]);
            wall.nextSectorIndex = stoi(m[4]);
            wall.textureIndex = stoi(m[5]);
            wall.repeatX = stof(m[6]);
            wall.repeatY = stof(m[7]);
            wall.panningX = stof(m[8]);
            wall.panningY = stof(m[9]);
            walls.push_back(wall);
            continue;
        }

        if(regex_match(line.c_str(), m, sectorRegex))
        {
            Sector sector;
            sector.startWall = stoi(m[1]);
            sector.numWalls = stoi(m[2]);
            sector.floor = stof(m[3]);
            sector.ceiling = stof(m[4]);
            sector.ceilingTextureIndex = stoi(m[5]);
            sector.floorTextureIndex = stoi(m[6]);
            sectors.push_back(sector);
            continue;
        }
        if(regex_match(line.c_str(), m, pstartRegex))
        {
            player.position = glm::vec3(stof(m[1]), stof(m[2]), stof(m[3]));
            player.angle = stof(m[4]);
            player.lastSector = stoi(m[5]);
            continue;
        }

        if(regex_match(line.c_str(), m, secnumRegex))
        {
            int secnum = stoi(m[1]);
            sectors.clear();
            sectors.shrink_to_fit();
            sectors.reserve(secnum);
            continue;
        }
        if(regex_match(line.c_str(), m, wallnumRegex))
        {
            int wallnum = stoi(m[1]);
            walls.clear();
            walls.shrink_to_fit();
            walls.reserve(wallnum);
            continue;
        }
    }

}
