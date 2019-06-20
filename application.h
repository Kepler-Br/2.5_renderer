#ifndef APPLICATION_H
#define APPLICATION_H
#include "window.h"
#include <stack>
#include <memory>
#include "inputManager.h"
#include "checkTimer.h"

class iGameState;

class iApplication
{
public:
    virtual ~iApplication()
    {
    }
    virtual void exit() = 0;
};

class iStateManager
{
protected:
    std::stack<std::unique_ptr<iGameState>> states;

    void checkOverflow()
    {
        const int maxStates = 50;
        if(states.size() > maxStates)
            throw std::runtime_error("Maximum number of states exeeded! (50)");
    }

    void checkUnderflow()
    {
        if( states.empty() )
            throw std::runtime_error("Popped last state!");
    }

public:
    void push(std::unique_ptr<iGameState> &state)
    {
        states.push(std::move(state));
        checkOverflow();
    }

    void pop()
    {
        states.pop();
        checkUnderflow();
    }

    void emplace(std::unique_ptr<iGameState> &state)
    {
        states.pop();
        states.push(std::move(state));
    }
};


class iGameState;

class Application: public iStateManager, public iApplication
{
private:
    CheckTimer frameTime;
    Window window;
    bool isRunning;
    InputManager *inputManager;

    void loop();

public:
    Application();
    int execute(std::unique_ptr<iGameState> state);
    Window &getWindow();
    void exit();
};


class iGameState
{
protected:
    iStateManager* stateManager;
    iApplication* app;
    Window &window;

public:
    iGameState(Application *app):
        stateManager(app),
        app(app),
        window(app->getWindow())
    {
    }

    virtual ~iGameState()
    {

    }

    virtual void input() = 0;

    virtual void update() = 0;
    virtual void lateUpdate() = 0;
    virtual void fixedUpdate() = 0;

    virtual void preRender() = 0;
    virtual void render() = 0;
    virtual void postRender() = 0;
};




#endif // APPLICATION_H
