#ifndef THEELDERWOODHILL_GAME_HPP
#define THEELDERWOODHILL_GAME_HPP

#include <SDL3/SDL.h>
#include "Map/Map.hpp"

class Game
{
public:
    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();

private:
    void handleEvents();
    void update(uint32_t deltaTime);
    void render(uint32_t deltaTime);

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    teh::map::Map* map;
    uint32_t lastTime;
};

#endif //THEELDERWOODHILL_GAME_HPP
