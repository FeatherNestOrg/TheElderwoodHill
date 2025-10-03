#include "Animation.hpp"

namespace teh::map
{
    void AnimationStateManager::update(const uint32_t deltaTime)
    {
        for (auto& [key, state] : m_States)
        {
            state.elapsedTime += deltaTime;
        }
    }

    AnimationState& AnimationStateManager::getState(const uint32_t tilesetIndex, const uint32_t animationIndex)
    {
        const uint64_t key = static_cast<uint64_t>(tilesetIndex) << 32 | animationIndex;
        return m_States[key];
    }
}
