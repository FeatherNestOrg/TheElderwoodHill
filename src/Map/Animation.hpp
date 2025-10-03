#ifndef THEELDERWOODHILL_ANIMATION_HPP
#define THEELDERWOODHILL_ANIMATION_HPP
#include <cstdint>
#include <ankerl/unordered_dense.h>

namespace teh::map
{
    struct AnimationState
    {
        uint32_t currentFrame{};
        uint32_t elapsedTime{};
    };

    class AnimationStateManager
    {
    public:
        void update(uint32_t deltaTime);
        AnimationState& getState(uint32_t tilesetIndex, uint32_t animationIndex);

    private:
        ankerl::unordered_dense::map<uint64_t, AnimationState> m_States;
    };
}
#endif //THEELDERWOODHILL_ANIMATION_HPP
