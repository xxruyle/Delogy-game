#include "animation_system.hpp"
#include "components.hpp"
#include <iostream>

void AnimationSystem::incrementAnimation(AnimationC &animation)
{
    animation.timeSinceLastFrameSwap += GetFrameTime();

    if (animation.timeSinceLastFrameSwap > animation.animationUpdateTime) {
        if (animation.atFrame[animation.curFrameSrc] < animation.numFrames - 1) {
            animation.atFrame[animation.curFrameSrc]++;
        }
        else {
            animation.atFrame[animation.curFrameSrc] = 0;
        }
        animation.timeSinceLastFrameSwap = 0.0f;
    }
}

void AnimationSystem::updatePlayerAnimation(InputSystem input, AnimationC &animation, bool movementState)
{
    int playerAnimation = input.getMovementDirection();
    switch (playerAnimation) {
    case MOVE_RIGHT:
        animation.curFrameSrc = (int)animationDirection::RIGHT;
        break;
    case MOVE_LEFT:
        animation.curFrameSrc = (int)animationDirection::LEFT;
        break;
    case MOVE_UP:
        animation.curFrameSrc = (int)animationDirection::UP;
        break;
    case MOVE_DOWN:
        animation.curFrameSrc = (int)animationDirection::DOWN;
        break;
    default:
        break;
    }

    incrementAnimation(animation);

    if (!movementState) { // if idle
        animation.atFrame[animation.curFrameSrc] = 0;
    }
}

void AnimationSystem::updateSprites(entt::basic_registry<> &registry)
{
    auto view = registry.view<const AnimationC, SpriteC>();

    for (auto entity : view) {
        auto &animation = view.get<AnimationC>(entity);
        auto &sprite = view.get<SpriteC>(entity);
        unsigned int atframe = animation.atFrame[animation.curFrameSrc];
        sprite.atlasPos.x = animation.frameSrcs[animation.curFrameSrc].x + atframe * (16 + ATLAS_SPRITE_PADDING);
        sprite.atlasPos.y = animation.frameSrcs[animation.curFrameSrc].y;
    }
}

void AnimationSystem::update(InputSystem input, entt::basic_registry<> &registry, entt::entity player)
{
    // test
    AnimationC &animation = registry.get<AnimationC>(player);
    PhysicsC &physics = registry.get<PhysicsC>(player);
    updatePlayerAnimation(input, animation, physics.moving);

    updateSprites(registry);
}
