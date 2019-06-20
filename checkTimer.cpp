#include "checkTimer.h"

#include <SDL2/SDL.h>

CheckTimer::CheckTimer()
{

}

float CheckTimer::check()
{
    timePassed = static_cast<float>(SDL_GetTicks() - lastTime);
    lastTime = SDL_GetTicks();
    return timePassed;
}
