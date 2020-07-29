#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "main_loop.h"
#include <cmath>
#include <stdexcept>

void Main_loop::pop_all_states()
{
    while(!this->state_stack.empty())
    {
        delete this->state_stack.top();
        this->state_stack.pop();
    }
}

Main_loop::Main_loop(const glm::ivec2 &resolution)
{
    this->sdl_instance = Sdl_wrapper(resolution);
}

Main_loop::~Main_loop()
{
    this->pop_all_states();
}

void Main_loop::push_state(State_base *state)
{
    this->state_stack.push(state);
}

void Main_loop::replace_state(State_base *state)
{
    if(this->state_stack.empty())
        throw std::runtime_error("Cannot replace when there is 0 states left.");
    state_to_delete = this->state_stack.top();
    this->state_stack.pop();
    this->push_state(state);
}

void Main_loop::pop_state()
{
    if(this->state_stack.size() == 1)
        throw std::runtime_error("Cannot pop when there is 1 state left.");
    state_to_delete = this->state_stack.top();
    this->state_stack.pop();
}

void Main_loop::start()
{
    this->is_running = true;
    while(this->is_running)
    {
        if(state_to_delete != nullptr)
        {
            delete state_to_delete;
            state_to_delete = nullptr;
        }
        State_base *current_state = this->state_stack.top();
        this->input_manager.update();
        current_state->on_event();
        current_state->engine_update();
        current_state->on_update();
        current_state->on_predraw();
        current_state->on_draw();
        current_state->on_postdraw();
        this->deltatime = this->calc_deltatime();
        this->limit_fps();
    }
    this->pop_all_states();
}

void Main_loop::stop()
{
    this->is_running = false;
}

float Main_loop::calc_deltatime()
{
    static float lastTime = 0;
    static float currentTime = 0;

    if (currentTime > lastTime)
        lastTime = currentTime;
    currentTime = SDL_GetTicks();
    return ((currentTime - lastTime) / 1000.0f);
}

void Main_loop::limit_fps()
{
    static uint start = SDL_GetTicks();
    if ((uint32_t(1000 / max_fps) > uint32_t(SDL_GetTicks() - start)))
        SDL_Delay(1000 / max_fps - (SDL_GetTicks() - start));
    start = SDL_GetTicks();
}

const float &Main_loop::get_deltatime() const
{
    return this->deltatime;
}

void Main_loop::set_max_fps(const int &fps)
{
    this->max_fps = fps;
}

Input_manager &Main_loop::get_input_manager()
{
    return this->input_manager;
}

Sdl_wrapper &Main_loop::get_sdl_instance() {
    return this->sdl_instance;
}

State_base *Main_loop::get_prev_state() {
    if(this->state_stack.size() == 1 || this->state_stack.empty())
        return nullptr;
    State_base *current_state = this->state_stack.top();
    this->pop_state();
    State_base *prev_state = this->state_stack.top();
    this->push_state(current_state);
    return prev_state;
}

