#ifndef WOLFENSHETIN_MAIN_LOOP_H
#define WOLFENSHETIN_MAIN_LOOP_H

#include "sdl_wrapper.h"
#include <stack>
#include "input_manager.h"
#include "state_base.h"

class Main_loop {
private:
    float deltatime;
    int max_fps = 15;
    bool is_running = false;
    std::stack<State_base *> state_stack;
    State_base *state_to_delete = nullptr;
    Sdl_wrapper sdl_instance;
    Input_manager input_manager;

    void pop_all_states();
    float calc_deltatime();
    void limit_fps();

public:
    Main_loop(const glm::ivec2 &resolution);
    ~Main_loop();

    void push_state(State_base *state);
    void replace_state(State_base *state);
    void pop_state();
    State_base *get_prev_state();
    const float &get_deltatime() const;
    void set_max_fps(const int &fps);
    Input_manager &get_input_manager();
    Sdl_wrapper &get_sdl_instance();
    void start();
    void stop();
};


#endif //WOLFENSHETIN_MAIN_LOOP_H
