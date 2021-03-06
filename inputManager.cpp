#include "inputManager.h"
#include <iostream>

InputManager InputManager::instance;

bool InputManager::wasKeyDown(uint keyID)
{
    auto it = prevKeyMap.find(keyID);
    if (it != prevKeyMap.end())
    {
        // Found the key.
        return it->second;
    }
    // Did not found the key.
    return false;
}

void InputManager::pressEvent(uint eventID)
{
    events[eventID] = true;
}

void InputManager::updateState()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                pressEvent(SDL_QUIT);
                break;
            case SDL_KEYDOWN:
                pressKey(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                releaseKey(event.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                pressKey(event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                releaseKey(event.button.button);
                break;
        }
    }
}

void InputManager::update()
{
    SDL_GetRelativeMouseState(&deltaMouseCoord.x, &deltaMouseCoord.y);
    //    for(auto it:keyMap)
    for (auto it = keyMap.begin(); it != keyMap.end(); it++)
    {
        prevKeyMap[it->first] = it->second;
    }
    updateState();
}

void InputManager::pressKey(uint keyID)
{
    keyMap[keyID] = true;
}

void InputManager::releaseKey(uint keyID)
{
    keyMap[keyID] = false;
}

glm::ivec2 InputManager::getDeltaMouseCoord()
{
    return deltaMouseCoord;
}

glm::ivec2 InputManager::getRelativeMouseCoord()
{
    SDL_GetMouseState(&relativeMouseCoord.x, &relativeMouseCoord.y);
    return relativeMouseCoord;
}

glm::ivec2 InputManager::getGlobalMouseCoord()
{
    SDL_GetGlobalMouseState(&globalMouseCoord.x, &globalMouseCoord.y);
    return globalMouseCoord;
}

bool InputManager::isKeyDown(uint keyID)
{
    auto it = keyMap.find(keyID);
    if (it != keyMap.end())
    {
        // Found the key.
        return it->second;
    }
    // Did not found the key.
    return false;
}

bool InputManager::isKeyReleased(uint keyID)
{
    if (wasKeyDown(keyID) == true && isKeyDown(keyID) == false)
    {
        return true;
    }
    return false;
}

bool InputManager::isKeyPressed(uint keyID)
{
    if (wasKeyDown(keyID) == false && isKeyDown(keyID) == true)
    {
        return true;
    }
    return false;
}

bool InputManager::isEventPending(uint eventID)
{
    auto it = events.find(eventID);
    if (it != events.end())
    {
        // Found the key.
        return it->second;
    }
    // Did not found the key.
    return false;
}

InputManager *InputManager::getInstance()
{
    return &instance;
}
