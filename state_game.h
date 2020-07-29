//
// Created by kepler-br on 6/10/20.
//

#ifndef WOLFENSHETIN_STATE_GAME_H
#define WOLFENSHETIN_STATE_GAME_H


#include "main_loop.h"
#include "state_base.h"
#include "sdl_wrapper.h"
#include "player.h"
#include "input_manager.h"
#include <SDL2/SDL_image.h>
#include "texture_holder.h"
#include "framebuffer.h"

class State_game: State_base
{
private:
    int render_map_state = 0;
    bool is_player_running = false;
    bool is_paused = false;
    Sdl_wrapper &sdl_wrapper;
    Input_manager &input_manager;
    Player player;
    Texture_holder texture_holder;
    Framebuffer framebuffer;
    const int framebuffer_divider = 2;

public:
    State_game(Main_loop &main_loop, Input_manager &input_manager, Sdl_wrapper &sdl_instance);
    void engine_update() override;
    void on_event() override;
    void on_update() override;
    void on_predraw() override;
    void on_draw() override;
    void on_postdraw() override;
    void preload() override;
};


#endif //WOLFENSHETIN_STATE_GAME_H
