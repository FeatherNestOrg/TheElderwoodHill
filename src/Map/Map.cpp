#include "Map.hpp"
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
        std::cout << "Map::load - Starting to load: " << filePath << std::endl;

        // Parse the TMX file using tmxparser
        auto result = tmx::Parser::parseFromFile(filePath);

        if (!result)
        {
            std::cerr << "Failed to parse TMX file: " << result.error() << std::endl;
            return false;
        }

        const auto& map = *result;

        std::cout << "Map::load - TMX file parsed successfully" << std::endl;
        std::cout << "Map::load - Map size: " << map.width << "x" << map.height << std::endl;
        std::cout << "Map::load - Tile size: " << map.tilewidth << "x" << map.tileheight << std::endl;
        std::cout << "Map::load - Number of tilesets: " << map.tilesets.size() << std::endl;
        std::cout << "Map::load - Number of layers: " << map.layers.size() << std::endl;

        // Get the base path for resolving relative tileset paths
        fs::path mapPath(filePath);
        std::string basePath = mapPath.parent_path().string();

        // Create pre-calculated render data
        std::cout << "Map::load - Creating render data..." << std::endl;
        m_RenderData = tmx::render::createRenderData(map, basePath);

        std::cout << "Map::load - Render data created:" << std::endl;
        std::cout << "  - Tilesets: " << m_RenderData.tilesets.size() << std::endl;
        std::cout << "  - Layers: " << m_RenderData.layers.size() << std::endl;

        // Count animations
        size_t totalAnimations = 0;
        for (const auto& tileset : m_RenderData.tilesets)
        {
            totalAnimations += tileset.animations.size();
            if (!tileset.animations.empty())
            {
                std::cout << "  - Tileset '" << tileset.name << "' has "
                    << tileset.animations.size() << " animations" << std::endl;
            }
        }
        std::cout << "  - Total animations: " << totalAnimations << std::endl;

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
            std::cout << "  - Layer '" << layer.name << "': " << layer.tiles.size() << " tiles" << std::endl;
        }
        std::cout << "  - Total renderable tiles: " << totalTiles
            << " (" << animatedTiles << " animated)" << std::endl;

        // Load all tileset textures
        std::cout << "Map::load - Loading tileset textures..." << std::endl;
        m_TilesetTextures.clear();

        for (size_t i = 0; i < m_RenderData.tilesets.size(); ++i)
        {
            const auto& tileset = m_RenderData.tilesets[i];

            std::cout << "Map::load - Loading tileset " << i << ": " << tileset.name << std::endl;
            std::cout << "  Image path: " << tileset.imagePath << std::endl;

            SDL_Texture* texture = IMG_LoadTexture(m_Renderer, tileset.imagePath.c_str());
            if (!texture)
            {
                std::cerr << "Failed to load tileset texture: " << tileset.imagePath
                    << " | SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }

            std::cout << "  Texture loaded successfully" << std::endl;
            m_TilesetTextures.push_back(texture);
        }

        std::cout << "Map loaded successfully!" << std::endl;
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
