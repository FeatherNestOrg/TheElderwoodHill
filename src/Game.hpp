#ifndef THEELDERWOODHILL_GAME_HPP
#define THEELDERWOODHILL_GAME_HPP

#include <SDL3/SDL.h>
#include "Map.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();

private:
    void handleEvents();
    void update();
    void render();

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Map* map;
};

#endif //THEELDERWOODHILL_GAME_HPP
