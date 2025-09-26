#include "Map.hpp"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Map::Map(SDL_Renderer* renderer) : renderer(renderer), tilesetTexture(nullptr) {}

Map::~Map() {
    if (tilesetTexture) {
        SDL_DestroyTexture(tilesetTexture);
    }
}

bool Map::load(const std::string& filePath) {
    tmxMap.ParseFile(filePath);

    auto tileset = tmxMap.GetTileset(0);
    if (!tileset) {
        std::cerr << "No tileset found in map." << std::endl;
        return false;
    }

    fs::path mapPath(filePath);
    fs::path tilesetImagePath = mapPath.parent_path() / tileset->GetImage()->GetSource();

    tilesetTexture = IMG_LoadTexture(renderer, tilesetImagePath.string().c_str());
    if (!tilesetTexture) {
        std::cerr << "Failed to load tileset texture: " << tilesetImagePath << " | " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

void Map::render() {
    if (!tilesetTexture) {
        return;
    }

    auto tileset = tmxMap.GetTileset(0);
    int tileWidth = tileset->GetTileWidth();
    int tileHeight = tileset->GetTileHeight();
    int columns = tileset->GetColumns();

    for (const auto& layer : tmxMap.GetLayers()) {
        if (layer->IsTileLayer()) {
            const tinytmx::TileLayer* tileLayer = layer->GetTileLayer();
            for (int y = 0; y < tmxMap.GetHeight(); ++y) {
                for (int x = 0; x < tmxMap.GetWidth(); ++x) {
                    const tinytmx::MapTile& mapTile = tileLayer->GetTile(x, y);
                    if (mapTile.gid == 0) {
                        continue;
                    }

                    int tilesetX = (mapTile.gid - 1) % columns;
                    int tilesetY = (mapTile.gid - 1) / columns;

                    SDL_Rect srcRect = { tilesetX * tileWidth, tilesetY * tileHeight, tileWidth, tileHeight };
                    SDL_Rect destRect = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };

                    SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);
                }
            }
        }
    }
}

