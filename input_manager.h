#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>


class Input_manager
{
private:
    // Key state of current frame.
    std::map<uint, bool> keyMap;
    // Key state of previous frame.
    std::map<uint, bool> prevKeyMap;
    std::map<uint, bool> events;
    glm::ivec2 relativeMouseCoord;
    glm::ivec2 globalMouseCoord;
    glm::ivec2 deltaMouseCoord;

    bool wasKeyDown(uint keyID);
    void pressEvent(uint eventID);
    void updateState();

public:
    void update();

    void pressKey(uint keyID);
    void releaseKey(uint keyID);

    // Gets.
    glm::ivec2 getDeltaMouseCoord();
    glm::ivec2 getRelativeMouseCoord();
    glm::ivec2 getGlobalMouseCoord();

    bool isKeyDown(uint keyID);
    bool isKeyReleased(uint keyID);
    bool isKeyPressed(uint keyID);
    bool isEventPending(uint eventID);
};
#endif // INPUTMANAGER_H
