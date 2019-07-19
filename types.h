#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>

namespace xenfa {

struct Player
{
    int lastSector;
    glm::vec3 position;
    glm::vec3 velocity;
    float angle,
    angleSin,
    angleCos,
    yawSin,
    yawCos,
    yaw = 0.0f;
};

struct Wall
{
    glm::vec2 point;
    int nextWallIndex;
    int nextSectorIndex;
    int textureIndex;
    float repeatX, repeatY;
    float panningX, panningY;
};

struct Sector
{
    int startWall;
    int numWalls;
    float floor, ceiling;
    int ceilingTextureIndex;
    int floorTextureIndex;
};

struct Bunch
{
    int wallIndex;
    int wallCount;
};


}

#endif // TYPES_H
