#include "main_loop.h"
#include "state_game.h"

int main()
{
    SDL_LogSetPriority(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_DEBUG);
    Main_loop main_loop({800, 600});
    auto *game = new State_game(main_loop, main_loop.get_input_manager(), main_loop.get_sdl_instance());
    game->preload();
    main_loop.push_state((State_base *)game);
//    try
//    {
        main_loop.start();
//    }
//    catch (const std::runtime_error &e)
//    {
//        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
//                                 "Runtime error",
//                                 e.what(),
//                                 NULL);
//    }
    return 0;
}
