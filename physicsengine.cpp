#include "physicsengine.h"

#include "types.h"

bool xenfa::PhysicsEngine::onSegment(const glm::vec2 &p, const glm::vec2 &q, const glm::vec2 &r)
{
    if (q.x <= glm::max(p.x, r.x) && q.x >= glm::min(p.x, r.x) &&
            q.y <= glm::max(p.y, r.y) && q.y >= glm::min(p.y, r.y))
        return true;
    return false;
}

int xenfa::PhysicsEngine::orientation(const glm::vec2 &p, const glm::vec2 &q, const glm::vec2 &r)
{
    int val = static_cast<int>((q.y - p.y) * (r.x - q.x) -
                               (q.x - p.x) * (r.y - q.y));

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

int xenfa::PhysicsEngine::checkObjectSector(const glm::vec3 &position, const int lastKnownSectorIndex)
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

bool xenfa::PhysicsEngine::isInsideSector(const glm::vec3 &coord, const int sectorIndex)
{
    const Sector sector = sectors[sectorIndex];

    const int startWallIndex = sector.startWall;
    Wall current = walls[startWallIndex];

    int linesIntersected = 0;
    for(int i = 0; i < sector.numWalls; i++)
    {
        const int nextWallIndex = current.nextWallIndex;
        Wall next = walls[nextWallIndex];
        if (((current.point.y>coord.y) != (next.point.y>coord.y)) &&
                (coord.x < (next.point.x-current.point.x) * (coord.y-current.point.y) / (next.point.y-current.point.y) + current.point.x) )
            linesIntersected ++;
        current = next;
    }
    bool insideXYSectorPlane = (linesIntersected%2 == 0? false : true);
    //        bool insideZSectorPlane = coord.z > sector.floor && coord.z < sector.ceiling;

    return insideXYSectorPlane/* && insideZSectorPlane*/;
}

bool xenfa::PhysicsEngine::doIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

glm::vec3 xenfa::PhysicsEngine::levelCollision(const glm::vec3 &oldPosition, const glm::vec3 &newPosition, const int sectorIndex)
{
    if(sectorIndex == -1)
        return newPosition;
    const Sector &sector = sectors[sectorIndex];
    Wall currentWall = walls[sector.startWall];
    for (int i = 0; i < sector.numWalls; i++)
    {
        Wall nextWall = walls[currentWall.nextWallIndex];
        if(currentWall.nextSectorIndex > -1)
        {
            currentWall = nextWall;
            continue;
        }
        bool isWallIntersects = doIntersect(currentWall.point, nextWall.point, oldPosition, newPosition);
        if(isWallIntersects)
        {
            return oldPosition;
        }
        currentWall = nextWall;
    }
    return newPosition;
}
