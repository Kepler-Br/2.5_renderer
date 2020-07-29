#ifndef WOLFENSHETIN_STATE_BASE_H
#define WOLFENSHETIN_STATE_BASE_H

class Main_loop;

class State_base
{
protected:
    Main_loop &main_loop;
    State_base* prev_state;

public:
    State_base(Main_loop &main_loop)
    : main_loop(main_loop)
    , prev_state(nullptr)
    {
    }
    State_base(Main_loop &main_loop, State_base *prev_state)
    : main_loop(main_loop)
    , prev_state(prev_state)
    {
    }
    virtual ~State_base()
    {
    }

    virtual void engine_update() = 0;
    virtual void on_event() = 0;
    virtual void on_update() = 0;
    virtual void on_fixed_update() = 0;
    virtual void on_predraw() = 0;
    virtual void on_draw() = 0;
    virtual void on_postdraw() = 0;
    virtual void preload() = 0;
};

#endif //WOLFENSHETIN_STATE_BASE_H
