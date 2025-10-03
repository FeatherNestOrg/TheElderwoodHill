#ifndef THEELDERWOODHILL_RENDERER_H
#define THEELDERWOODHILL_RENDERER_H

#include <SDL3/SDL_render.h>
#include <tmx/tmx.hpp>
#include "Animation.hpp"

namespace teh::map
{
    /**
     * @brief Handles rendering of TMX maps with animation support
     */
    class Renderer
    {
    public:
        explicit Renderer(SDL_Renderer* sdlRenderer);
        ~Renderer();

        /**
         * @brief Render the entire map with animations
         * @param renderData Pre-calculated render data from tmxparser
         * @param tilesetTextures Vector of loaded tileset textures
         * @param deltaTime Time elapsed since last frame in milliseconds
         */
        void render(const tmx::render::MapRenderData& renderData,
                   const std::vector<SDL_Texture*>& tilesetTextures,
                   uint32_t deltaTime);

        /**
         * @brief Reset all animation states
         */
        void resetAnimations();

    private:
        /**
         * @brief Render a single layer
         */
        void renderLayer(const tmx::render::LayerRenderData& layer,
                        const tmx::render::MapRenderData& renderData,
                        const std::vector<SDL_Texture*>& tilesetTextures,
                        uint32_t deltaTime);

        SDL_Renderer* m_SdlRenderer;
        AnimationStateManager m_AnimationStates;
    };
} // namespace teh::map

#endif //THEELDERWOODHILL_RENDERER_H
