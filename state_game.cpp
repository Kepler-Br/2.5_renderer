#include "state_game.h"
#include <texture_loader.h>
#include <iostream> //DELETEME
State_game::State_game(Main_loop &main_loop, Input_manager &input_manager, Sdl_wrapper &sdl_instance) :
        State_base(main_loop, main_loop.get_prev_state()),
        sdl_wrapper(sdl_instance),
        input_manager(input_manager),
        player({1500, 1560}, 360),
        texture_holder(10),
        raycaster(world, lookup),
        framebuffer(sdl_wrapper),
        renderer(world, sdl_wrapper, player, raycaster, lookup, texture_holder, framebuffer, 1)
{
    const glm::ivec2 framebuffer_resolution = this->sdl_wrapper.get_resolution() / this->framebuffer_divider;
    this->framebuffer.init(framebuffer_resolution);
    this->lookup.init(this->world.get_block_size(), framebuffer_resolution.x);
//    SDL_ShowCursor(SDL_DISABLE);
//    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void State_game::engine_update()
{

}

void State_game::on_event()
{
    const float deltatime = this->main_loop.get_deltatime();
//    const glm::ivec2 mouse_delta = this->input_manager.getDeltaMouseCoord();
//    this->player.add_x_view_angle(mouse_delta.x/500.0f);
//    this->player.add_y_view_angle(-mouse_delta.y*1.8f);
    if(this->input_manager.isKeyDown(SDLK_w))
    {
        const glm::vec2 forward = {this->lookup.cos(this->player.get_x_view_angle()),
                                   this->lookup.sin(this->player.get_x_view_angle())};
        int speed;
        if(this->is_player_running)
            speed = this->player.get_running_speed();
        else
            speed = this->player.get_speed();
        glm::vec2 velocity = {forward.x * speed * deltatime,
                              forward.y * speed * deltatime};
        this->player.add_position(velocity);
    }
    if(this->input_manager.isKeyDown(SDLK_s))
    {
        const glm::vec2 forward = {this->lookup.cos(this->player.get_x_view_angle()),
                                   this->lookup.sin(this->player.get_x_view_angle())};
        int speed;
        if(this->is_player_running)
            speed = this->player.get_running_speed();
        else
            speed = this->player.get_speed();
        glm::vec2 velocity = {forward.x * speed * deltatime,
                              forward.y * speed * deltatime};
        this->player.add_position(-velocity);
    }
    if(this->input_manager.isKeyDown(SDLK_a))
    {
        const glm::vec2 left = {this->lookup.sin(this->player.get_x_view_angle()),
                                 -this->lookup.cos(this->player.get_x_view_angle())};
        int speed;
        if(this->is_player_running)
            speed = this->player.get_running_speed();
        else
            speed = this->player.get_speed();
        glm::vec2 velocity = {left.x * speed * deltatime,
                              left.y * speed * deltatime};
        this->player.add_position(velocity);
    }
    if(this->input_manager.isKeyDown(SDLK_d))
    {
        const glm::vec2 right = {-this->lookup.sin(this->player.get_x_view_angle()),
                                 this->lookup.cos(this->player.get_x_view_angle())};
        int speed;
        if(this->is_player_running)
            speed = this->player.get_running_speed();
        else
            speed = this->player.get_speed();
        glm::vec2 velocity = {right.x * speed * deltatime,
                              right.y * speed * deltatime};
        this->player.add_position(velocity);
    }
    if(this->input_manager.isKeyDown(SDLK_RIGHT))
    {
        this->player.add_x_view_angle(this->lookup.angle180 * deltatime);
        const int angle = this->player.get_x_view_angle();
        if (angle>this->lookup.angle360)
            this->player.add_x_view_angle(-this->lookup.angle360);
    }
    if(this->input_manager.isKeyDown(SDLK_LEFT))
    {
        this->player.add_x_view_angle(-this->lookup.angle180 * deltatime);
        const int angle = this->player.get_x_view_angle();
        if (angle<0)
            this->player.add_x_view_angle(this->lookup.angle360);
    }
    if(this->input_manager.isKeyDown(SDLK_UP))
        this->framebuffer.offset_center(glm::ivec2(0, 640*deltatime));
    if(this->input_manager.isKeyDown(SDLK_DOWN))
        this->framebuffer.offset_center(glm::ivec2(0, -640*deltatime));
    if(this->input_manager.isKeyDown(SDLK_ESCAPE))
        this->main_loop.stop();
    if(this->input_manager.isEventPending(SDL_QUIT))
        this->main_loop.stop();
    if(this->input_manager.isKeyPressed(SDLK_TAB))
    {
        this->render_map_state++;
        if(this->render_map_state > 2)
            this->render_map_state = 0;
    }
    if(this->input_manager.isKeyPressed(SDLK_LSHIFT))
        this->is_player_running = true;
    if(this->input_manager.isKeyReleased(SDLK_LSHIFT))
        this->is_player_running = false;
}

void State_game::on_update()
{
    std::cout << "FPS: " << 1.0f/this->main_loop.get_deltatime() << std::endl;
    const Block player_block = this->world.get_block({this->player.get_position().x/this->world.get_block_size(),
                                                     this->player.get_position().y/this->world.get_block_size()});
    this->player.set_height(player_block.floor_height + 512);
//    this->player.setHeight(1012);
}

void State_game::on_predraw()
{
//    this->sdl_wrapper.clear();

//    this->sdl_wrapper.clear_framebuffer();
}

void State_game::on_draw()
{
    this->sdl_wrapper.set_color(255, 255, 255);
//    this->renderer.draw_world();

//    const glm::ivec2 &resolution = this->framebuffer.get_resolution();
    this->framebuffer.lock();
    this->framebuffer.clear();
    this->renderer.render();
//    for(int x = 0; x < resolution.x; x++)
//    {
//        for(int y = 0; y < resolution.y; y++)
//        {
//            glm::vec3 color = {1.0f*float(x)/resolution.x,
//                              1.0f*float(y)/resolution.y,
//                              1.0f*float(x)/resolution.x};
//            this->framebuffer.set_pixel(color, glm::ivec2(x, y));
//        }
//    }
    this->framebuffer.unlock();
    this->framebuffer.put();
    if(render_map_state == 1)
        this->renderer.render_map(false, false, 0.05f);
    else if(render_map_state == 2)
        this->renderer.render_map(false, true, 0.05f);
//    this->sdl_wrapper.lock_framebuffer();
//    this->sdl_wrapper.clear_framebuffer();
//    this->renderer.render();
//    this->sdl_wrapper.unlock_framebuffer();
//    this->sdl_wrapper.put_framebuffer();
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

}

void State_game::on_postdraw()
{

    this->sdl_wrapper.present();
}

void State_game::preload()
{
    Texture_loader loader;
    std::string texture_paths[] = {/*"./image_packer/BAYONETTA.xft",*/
                                  "./image_packer/BRICK_WALL.xft",
                                  "./image_packer/BRICK_WALL2.xft",
                                  "./image_packer/DOOR2_4.xft",
                                  "./image_packer/RW24_2.xft",
                                  "./image_packer/TITLEPIC.xft",
                                  "./image_packer/CEIL1_1.xft",
                                  "./image_packer/grass_block_side.xft",
                                  "./image_packer/dirt.xft",
                                  "./image_packer/uv_grid.xft"};
    for(auto path: texture_paths)
    {
        std::string texture_name;
        Texture *texture;
        std::tie(texture, texture_name) = loader.load(path);
        this->texture_holder.add_texture(texture_name, texture);
    }
}
