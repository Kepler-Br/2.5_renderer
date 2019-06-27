#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>

#include <glm/glm.hpp>

#include "types.h"


namespace xenfa
{

class PhysicsEngine
{
    std::vector<Wall> &walls;
    std::vector<Sector> &sectors;


    bool onSegment(const glm::vec2 &p, const glm::vec2 &q, const glm::vec2 &r);
    int orientation(const glm::vec2 &p, const glm::vec2 &q, const glm::vec2 &r);
    bool doIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

public:
    PhysicsEngine(std::vector<Wall> &walls, std::vector<Sector> &sectors):
        walls(walls),
        sectors(sectors)
    {}

    int checkObjectSector(const glm::vec3 &position, const int lastKnownSectorIndex);
    bool isInsideSector(const glm::vec3 &coord, const int sectorIndex);
    glm::vec3 levelCollision(const glm::vec3 &oldPosition, const glm::vec3 &newPosition, const int sectorIndex);
};

}

#endif // PHYSICSENGINE_H
