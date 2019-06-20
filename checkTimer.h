#ifndef CHECKTIMER_H
#define CHECKTIMER_H

#include <inttypes.h>

class CheckTimer
{
    float timePassed = 0.0f;
    uint32_t lastTime = 0.0f;
public:
    CheckTimer();
    /**
     *  \brief Returns time since last check.
     */
    float check();
};

#endif // CHECKTIMER_H
