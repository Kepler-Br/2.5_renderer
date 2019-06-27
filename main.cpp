#include "application.h"
#include "playState.h"
#include "iStateManager.h"

#include <memory>

int main()
{
    Application app;
    return app.execute(std::make_unique<xenfa::PlayState>(&app));
}
