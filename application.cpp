#include "application.h"
#include <stdexcept>
#include <SDL2/SDL.h>

#include "window.h"

void Application::loop()
{
    const float stepTime = 30.0;
    static float lag;
    inputManager->update();



    this->states.top()->preRender();
    this->states.top()->render();
    this->states.top()->postRender();

    this->states.top()->update();
    while(lag > stepTime)
    {
        this->states.top()->input();
        this->states.top()->fixedUpdate();
        lag -= stepTime;
    }
    this->states.top()->lateUpdate();

    lag += frameTime.check();
}

Application::Application():
    window(800, 600),
    inputManager(InputManager::getInstance())
{
    window.setWindowTitle("Dukenukem portal rendering test.");
}



int Application::execute(std::unique_ptr<iGameState> state)
{
    isRunning = true;
    states.push(std::move(state));
    try
    {
        while(isRunning)
            loop();
    }
    catch (const std::runtime_error &error)
    {
        puts(error.what());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Runtime error",
                                 error.what(),
                                 nullptr);
    }
    return 0;
}

Window &Application::getWindow()
{
    return window;
}

void Application::exit()
{
    isRunning = false;
}
