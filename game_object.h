#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <glm/vec3.hpp>

class Game_object
{
public:
    std::string name;
    int id;
    glm::vec3 position;
};

#endif // GAME_OBJECT_H
