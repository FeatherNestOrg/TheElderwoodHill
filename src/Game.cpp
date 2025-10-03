#include "Game.hpp"
#include <iostream>

static std::string ASSETS_PATH(TEH_ASSETS_PATH);

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), map(nullptr), lastTime(0)
{
}

Game::~Game()
{
    cleanup();
}

bool Game::init()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("古树之丘", 640, 480, 0);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    map = new teh::map::Map(renderer);
    if (!map->load(ASSETS_PATH + "maps/tests/dungeon/dungeon.tmx"))
    {
        std::cerr << "Failed to load map." << std::endl;
        return false;
    }

    isRunning = true;
    lastTime = SDL_GetTicks();
    return true;
}

void Game::run()
{
    while (isRunning)
    {
        // Calculate delta time
        uint32_t currentTime = SDL_GetTicks();
        uint32_t deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        handleEvents();
        update(deltaTime);
        render(deltaTime);

        // Small delay to avoid high CPU usage (~60 FPS)
        SDL_Delay(16);
    }
}

void Game::cleanup()
{
    delete map;
    map = nullptr;

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            isRunning = false;
        }
        else if (e.type == SDL_EVENT_KEY_DOWN)
        {
            if (e.key.key == SDLK_ESCAPE)
            {
                isRunning = false;
            }
        }
    }
}

void Game::update(uint32_t deltaTime)
{
    // Game logic updates will go here
}

void Game::render(uint32_t deltaTime)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (map)
    {
        map->render(deltaTime);
    }

    SDL_RenderPresent(renderer);
}
