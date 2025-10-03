#include "Map.hpp"
#include "../Utils/Logger.hpp"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace teh::map
{
    Map::Map(SDL_Renderer* renderer)         : m_Renderer(renderer)
          , m_MapRenderer(renderer)
          , m_Loaded(false)
    {
    }

    Map::~Map()
    {
        // Clean up all tileset textures
        for (auto* texture : m_TilesetTextures)
        {
            if (texture)
            {
                SDL_DestroyTexture(texture);
            }
        }
        m_TilesetTextures.clear();
    }

    bool Map::load(const std::string& filePath)
    {
        TEH_MAP_LOG(INFO, "Starting to load map: {}", filePath);

        // Parse the TMX file using tmxparser
        auto result = tmx::Parser::parseFromFile(filePath);

        if (!result)
        {
            TEH_MAP_LOG(ERROR, "Failed to parse TMX file: {}", result.error());
            return false;
        }

        const auto& map = *result;

        TEH_MAP_LOG(INFO, "TMX file parsed successfully");
        TEH_MAP_LOG(DEBUG, "Map size: {}x{}", map.width, map.height);
        TEH_MAP_LOG(DEBUG, "Tile size: {}x{}", map.tilewidth, map.tileheight);
        TEH_MAP_LOG(DEBUG, "Number of tilesets: {}", map.tilesets.size());
        TEH_MAP_LOG(DEBUG, "Number of layers: {}", map.layers.size());

        // Get the base path for resolving relative tileset paths
        fs::path mapPath(filePath);
        std::string basePath = mapPath.parent_path().string();

        // Create pre-calculated render data
        TEH_MAP_LOG(INFO, "Creating render data...");
        m_RenderData = tmx::render::createRenderData(map, basePath);

        TEH_MAP_LOG(DEBUG, "Render data created - Tilesets: {}, Layers: {}", m_RenderData.tilesets.size(), m_RenderData.layers.size());

        // Count animations
        size_t totalAnimations = 0;
        for (const auto& tileset : m_RenderData.tilesets)
        {
            totalAnimations += tileset.animations.size();
            if (!tileset.animations.empty())
            {
                TEH_MAP_LOG(DEBUG, "Tileset '{}' has {} animations", tileset.name, tileset.animations.size());
            }
        }
        TEH_MAP_LOG(DEBUG, "Total animations: {}", totalAnimations);

        size_t totalTiles = 0;
        size_t animatedTiles = 0;
        for (const auto& layer : m_RenderData.layers)
        {
            totalTiles += layer.tiles.size();
            for (const auto& tile : layer.tiles)
            {
                if (tile.isAnimated)
                {
                    animatedTiles++;
                }
            }
            TEH_MAP_LOG(DEBUG, "Layer '{}': {} tiles", layer.name, layer.tiles.size());
        }
        TEH_MAP_LOG(DEBUG, "Total renderable tiles: {} ({} animated)", totalTiles, animatedTiles);

        // Load all tileset textures
        TEH_MAP_LOG(INFO, "Loading tileset textures...");
        m_TilesetTextures.clear();

        for (size_t i = 0; i < m_RenderData.tilesets.size(); ++i)
        {
            const auto& tileset = m_RenderData.tilesets[i];

            TEH_MAP_LOG(DEBUG, "Loading tileset {}: '{}' from {}", i, tileset.name, tileset.imagePath);

            SDL_Texture* texture = IMG_LoadTexture(m_Renderer, tileset.imagePath.c_str());
            if (!texture)
            {
                TEH_RESOURCE_LOG(ERROR, "Failed to load tileset texture: {} | SDL Error: {}", tileset.imagePath, SDL_GetError());
                return false;
            }

            TEH_RESOURCE_LOG(DEBUG, "Texture loaded successfully: {}", tileset.imagePath);
            m_TilesetTextures.push_back(texture);
        }

        TEH_MAP_LOG(INFO, "Map loaded successfully!");
        m_Loaded = true;
        return true;
    }

    void Map::render(uint32_t deltaTime)
    {
        if (!m_Loaded)
        {
            return;
        }

        // Delegate rendering to the Renderer class
        m_MapRenderer.render(m_RenderData, m_TilesetTextures, deltaTime);
    }
}
