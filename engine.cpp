#include "engine.h"

int Engine::checkObjectSector(const glm::vec3 &position, const int lastKnownSectorIndex)
{
    if(lastKnownSectorIndex != -1)
    {
        // If sector wasn't changed.
        if(isInsideSector(position, lastKnownSectorIndex))
            return lastKnownSectorIndex;
        const Sector &lastKnownSector = sectors[lastKnownSectorIndex];

        Wall currentWall = walls[lastKnownSector.startWall];
        // Check neighbour sectors.
        for(int i = 0; i < lastKnownSector.numWalls; i++)
        {
            int nextSectorIndex = currentWall.nextSectorIndex;
            // Wall is a portal.
            if(nextSectorIndex != -1)
            {
                if(isInsideSector(position, nextSectorIndex))
                    return nextSectorIndex;
            }
        }

    }
    // If object is not in neighbour sectors then check all sectors
    for(int i = 0; i < sectors.size(); i++)
    {
        if( i == lastKnownSectorIndex)
            continue;
        if(isInsideSector(position, i))
            return i;
    }

    return -1;
}

void Engine::fixedUpdate()
{

}

void Engine::input()
{
    float playerSpeed = 5.0f;
    if(inputManager.isKeyDown(SDLK_w))
    {
        glm::vec3 newPlayerPosition = glm::vec3(player.position.x + cos(player.angle)*playerSpeed,
                                                player.position.y + sin(player.angle)*playerSpeed,
                                                player.position.z);
        levelCollision(newPlayerPosition);
    }
    if(inputManager.isKeyDown(SDLK_s))
    {
        glm::vec3 newPlayerPosition = glm::vec3(player.position.x - cos(player.angle)*playerSpeed,
                                                player.position.y - sin(player.angle)*playerSpeed,
                                                player.position.z);
        levelCollision(newPlayerPosition);
    }

    if(inputManager.isKeyDown(SDLK_a))
        player.angle -= M_PI/20.0f;
    if(inputManager.isKeyDown(SDLK_d))
        player.angle += M_PI/20.0f;
    player.lastSector = checkObjectSector(player.position, player.lastSector);
}

void Engine::levelCollision(const glm::vec3 &newPosition)
{
    if(player.lastSector == -1)
    {
        player.position = newPosition;
        return;
    }
    const Sector &playerSector = sectors[player.lastSector];
    Wall currentWall = walls[playerSector.startWall];
    for (int i = 0; i < playerSector.numWalls; i++)
    {
        if(currentWall.nextSectorIndex != -1)
            continue;
        Wall nextWall = walls[currentWall.nextWallIndex];
        bool isWallIntersects = overlap(glm::vec2(currentWall.point.x, nextWall.point.x),
                                        glm::vec2(player.position.x, newPosition.x)) &&
                overlap(glm::vec2(currentWall.point.y, nextWall.point.y),
                        glm::vec2(player.position.y, newPosition.y));
        if(isWallIntersects)
        {
            return;
        }
        currentWall = nextWall;
    }
    player.position = newPosition;
}

void Engine::render()
{
    for(auto &sector : sectors)
    {
        const int &startWallIndex = sector.startWall;
        const int &totalWalls = sector.numWalls;
        const Wall startWall = walls[startWallIndex];
        Wall currentWall = startWall;
        const glm::vec3 bias = glm::vec3(100.0f, 100.0f, 100.0f);
        window.setColor(glm::vec3(1.0f));
        window.square(player.position+bias-glm::vec3(3.0f), glm::vec2(6.0f));
        constexpr float lookLength = 20.0f;
        glm::vec2 playerSight = glm::normalize(player.position-glm::vec3(player.position.x + cos(player.angle)*lookLength,
                                                                         player.position.y + sin(player.angle)*lookLength, 0.0f));
        window.line(player.position+bias, glm::vec3(player.position.x + cos(player.angle)*lookLength, player.position.y + sin(player.angle)*lookLength, 0.0f)+bias);
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

void Engine::readMap(const std::string &path)
{
    using namespace std;

    ifstream mapFile;
    mapFile.open(path);
    string line = "1";
    regex pstartRegex("^ *pstart +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) +(\\d+) *$");
    regex sectorRegex("^ *s +(\\d+) +(\\d+) +(-?\\d+\\.?\\d*) +(-?\\d+\\.?\\d*) *$");
    regex wallRegex(" *w +(\\-?\\d+\\.?\\d*) +(\\-?\\d+\\.?\\d*) +(\\d+) +(\\-?\\d+) *");

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
