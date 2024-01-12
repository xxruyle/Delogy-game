#include "animation_system.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include "raymath.h"
#include <iostream>

void AnimationSystem::incrementAnimation(AnimationC &animation)
{
    animation.timeSinceLastFrameSwap += GetFrameTime();

    if (animation.timeSinceLastFrameSwap > animation.animationUpdateTime) {
        if (animation.atFrame[animation.curFrameSrc] < animation.framesPerRow - 1) {
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
    /* int playerAnimation = input.getMovementDirection(); */
    Vector2 direction = input.getDirectionVector();

    if (direction.x < 0 && !(direction.y > 0 || direction.y < 0)) {
        animation.curFrameSrc = (int)animationDirection::LEFT;
    }
    if (direction.x > 0 && !(direction.y > 0 || direction.y < 0)) {
        animation.curFrameSrc = (int)animationDirection::RIGHT;
    }
    if (direction.y < 0 && !(direction.x > 0 || direction.x < 0)) {
        animation.curFrameSrc = (int)animationDirection::UP;
    }
    if (direction.y > 0 && !(direction.x > 0 || direction.x < 0)) {
        animation.curFrameSrc = (int)animationDirection::DOWN;
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
        int distanceToNextSprite = animation.frameSrcs[animation.curFrameSrc].width + ATLAS_SPRITE_PADDING;
        unsigned int atframe = animation.atFrame[animation.curFrameSrc];
        sprite.atlasPos.x = animation.frameSrcs[animation.curFrameSrc].x + atframe * (distanceToNextSprite);
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
