#include "Game.hpp"
#include <iostream>

static std::string ASSETS_PATH(TEH_ASSETS_PATH);

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), map(nullptr) {}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("TEH", 640, 480, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    map = new Map(renderer);
    if (!map->load(ASSETS_PATH + "maps/tests/dungeon.tmx")) {
        std::cerr << "Failed to load map." << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Game::cleanup() {
    delete map;
    map = nullptr;

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            isRunning = false;
        }
    }
}

void Game::update() {
    // Game logic updates will go here
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (map) {
        map->render();
    }

    SDL_RenderPresent(renderer);
}
