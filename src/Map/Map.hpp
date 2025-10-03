#ifndef THEELDERWOODHILL_MAP_HPP
#define THEELDERWOODHILL_MAP_HPP

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <tmx/tmx.hpp>
#include "Renderer.hpp"

namespace teh::map
{
    /**
     * @brief Manages map data loading and rendering
     */
    class Map
    {
    public:
        explicit Map(SDL_Renderer* renderer);
        ~Map();

        /**
         * @brief Load a TMX map file
         * @param filePath Path to the .tmx file
         * @return true if loaded successfully, false otherwise
         */
        bool load(const std::string& filePath);

        /**
         * @brief Render the map with animations
         * @param deltaTime Time elapsed since last frame in milliseconds
         */
        void render(uint32_t deltaTime);

        /**
         * @brief Check if a map is currently loaded
         */
        bool isLoaded() const { return m_Loaded; }

        /**
         * @brief Get map dimensions in pixels
         */
        uint32_t getPixelWidth() const { return m_RenderData.pixelWidth; }
        uint32_t getPixelHeight() const { return m_RenderData.pixelHeight; }

        /**
         * @brief Get map dimensions in tiles
         */
        uint32_t getWidth() const { return m_RenderData.mapWidth; }
        uint32_t getHeight() const { return m_RenderData.mapHeight; }

    private:
        SDL_Renderer* m_Renderer;
        Renderer m_MapRenderer;
        tmx::render::MapRenderData m_RenderData;
        std::vector<SDL_Texture*> m_TilesetTextures;
        bool m_Loaded;
    };
}
#endif //THEELDERWOODHILL_MAP_HPP
