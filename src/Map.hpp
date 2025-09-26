#ifndef THEELDERWOODHILL_MAP_HPP
#define THEELDERWOODHILL_MAP_HPP

#include <SDL3/SDL.h>
#include <string>
#include <tinytmx/tinytmx.hpp>

class Map {
public:
    Map(SDL_Renderer* renderer);
    ~Map();

    bool load(const std::string& filePath);
    void render();

private:
    SDL_Renderer* renderer;
    tinytmx::Map tmxMap;
    SDL_Texture* tilesetTexture;
};

#endif //THEELDERWOODHILL_MAP_HPP

