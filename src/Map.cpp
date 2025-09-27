#include "Map.hpp"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>
#include "tinytmx.hpp"

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

    if (!tileset->GetImage()) {
        std::cerr << "Tileset image not found." << std::endl;
        return false;
    }

    fs::path mapPath(filePath);
    fs::path tilesetImagePath = mapPath.parent_path() / tileset->GetImage()->GetSource();

    tilesetTexture = IMG_LoadTexture(renderer, tilesetImagePath.string().c_str());
    if (!tilesetTexture) {
        std::cerr << "Failed to load tileset texture: " << tilesetImagePath << " | " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Map::render() {
    if (!tilesetTexture) {
        return;
    }

    auto tileset = tmxMap.GetTileset(0);
    if (!tileset) return;
    auto tileWidth = tileset->GetTileWidth();
    auto tileHeight = tileset->GetTileHeight();
    auto columns = tileset->GetColumns();

    for (const auto& layer : tmxMap.GetTileLayers()) {
        const auto* dataChunk = layer->GetDataTileFiniteMap();
        if (!dataChunk) {
            continue;
        }
        for (int y = 0; y < dataChunk->GetHeight(); ++y) {
            for (int x = 0; x < dataChunk->GetWidth(); ++x) {
                const tinytmx::MapTile& mapTile = dataChunk->GetTile(x, y);
                if (mapTile.gid == 0) {
                    continue;
                }

                int tilesetX = (static_cast<int>(mapTile.gid) - 1) % static_cast<int>(columns);
                int tilesetY = (static_cast<int>(mapTile.gid) - 1) / static_cast<int>(columns);

                SDL_FRect srcRect = { static_cast<float>(tilesetX * tileWidth), static_cast<float>(tilesetY * tileHeight), static_cast<float>(tileWidth), static_cast<float>(tileHeight) };
                SDL_FRect destRect = { static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight), static_cast<float>(tileWidth), static_cast<float>(tileHeight) };

                SDL_RenderTexture(renderer, tilesetTexture, &srcRect, &destRect);
            }
        }
    }
}
