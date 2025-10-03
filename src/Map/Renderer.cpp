#include "Renderer.hpp"
#include <iostream>

namespace teh::map
{
    Renderer::Renderer(SDL_Renderer* sdlRenderer)
        : m_SdlRenderer(sdlRenderer)
    {
    }

    Renderer::~Renderer() = default;

    void Renderer::render(const tmx::render::MapRenderData& renderData,
                         const std::vector<SDL_Texture*>& tilesetTextures,
                         uint32_t deltaTime)
    {
        // Render all layers
        for (const auto& layer : renderData.layers)
        {
            renderLayer(layer, renderData, tilesetTextures, deltaTime);
        }
    }

    void Renderer::renderLayer(const tmx::render::LayerRenderData& layer,
                               const tmx::render::MapRenderData& renderData,
                               const std::vector<SDL_Texture*>& tilesetTextures,
                               uint32_t deltaTime)
    {
        // Skip invisible layers
        if (!layer.visible)
            return;

        // Render all tiles in the layer
        for (const auto& tile : layer.tiles)
        {
            // Get the tileset texture
            if (tile.tilesetIndex >= tilesetTextures.size())
            {
                continue;
            }

            SDL_Texture* texture = tilesetTextures[tile.tilesetIndex];
            if (!texture)
            {
                continue;
            }

            SDL_FRect srcRect;
            SDL_FRect destRect;

            if (tile.isAnimated && tile.animationIndex != static_cast<uint32_t>(-1))
            {
                // Get animation info
                const auto& tilesetInfo = renderData.tilesets[tile.tilesetIndex];
                if (tile.animationIndex >= tilesetInfo.animations.size())
                {
                    continue;
                }

                const auto& animation = tilesetInfo.animations[tile.animationIndex];

                // Get or create animation state
                auto& state = m_AnimationStates.getState(tile.tilesetIndex, tile.animationIndex);

                // Update animation
                state.elapsedTime += deltaTime;

                // Use flattened lookup to get current frame index - O(1) instead of O(n)
                const uint32_t timeInCycle = state.elapsedTime % animation.totalDuration;
                const uint32_t frameIndex = animation.getFrameIndexAtTime(timeInCycle);

                // Use the current animation frame
                const auto& frame = animation.frames[frameIndex];
                srcRect = {
                    static_cast<float>(frame.srcX),
                    static_cast<float>(frame.srcY),
                    static_cast<float>(tile.srcW),
                    static_cast<float>(tile.srcH)
                };
            }
            else
            {
                // Static tile - use pre-calculated source rect
                srcRect = {
                    static_cast<float>(tile.srcX),
                    static_cast<float>(tile.srcY),
                    static_cast<float>(tile.srcW),
                    static_cast<float>(tile.srcH)
                };
            }

            // Destination is always the same
            destRect = {
                static_cast<float>(tile.destX),
                static_cast<float>(tile.destY),
                static_cast<float>(tile.destW),
                static_cast<float>(tile.destH)
            };

            // Apply opacity if not fully opaque
            if (tile.opacity < 1.0f)
            {
                SDL_SetTextureAlphaModFloat(texture, tile.opacity);
            }

            SDL_RenderTexture(m_SdlRenderer, texture, &srcRect, &destRect);

            // Reset opacity
            if (tile.opacity < 1.0f)
            {
                SDL_SetTextureAlphaModFloat(texture, 1.0f);
            }
        }
    }

    void Renderer::resetAnimations()
    {
        m_AnimationStates = AnimationStateManager{};
    }
}
