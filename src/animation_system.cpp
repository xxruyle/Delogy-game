#include "animation_system.hpp"
#include "ecs_registry.hpp"
#include "input_system.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"

void AnimationSystem::incrementAnimation(AnimationC& animation)
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

void AnimationSystem::updatePlayerAnimation(AnimationC& animation, PhysicsC& physics)
{
	Vector2 direction = InputSystem::getDirectionVector();

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

	if (!physics.moving()) { // if idle
		animation.atFrame[animation.curFrameSrc] = 0;
	}
}

void AnimationSystem::updateNPCAnimations(AnimationC& animation, PhysicsC& physics)
{

	Vector2 dir = Vector2Normalize(physics.velocity);
	float dirDot = Vector2Angle(Vector2{0, -1}, dir) * 180.0f / PI;

	if (physics.moving()) {
		bool UP = (dirDot >= 157 && dirDot <= 180) || (dirDot >= 0 && dirDot <= 22);
		bool RIGHT = dirDot > 22 && dirDot < 67;
		bool DOWN = dirDot > 67 && dirDot < 112;
		bool LEFT = dirDot > 112 && dirDot < 157;

		if (UP) // UP
		{
			animation.curFrameSrc = (int)animationDirection::UP;
		}
		else if (RIGHT) { // RIGHT
			animation.curFrameSrc = (int)animationDirection::RIGHT;
		}
		else if (DOWN) { // DOWN
			animation.curFrameSrc = (int)animationDirection::DOWN;
		}
		else if (LEFT) { // LEFT
			animation.curFrameSrc = (int)animationDirection::LEFT;
		}
		incrementAnimation(animation);
	}
	else { // if idle
		animation.atFrame[animation.curFrameSrc] = 0;
	}
}

void AnimationSystem::updateSprites()
{
	auto view = ECS::registry.view<AnimationC, SpriteC>();
	float ATLAS_SPRITE_PADDING = Slua::lua["ATLAS_SPRITE_PADDING"].get_or(0.0f);

	for (auto entity : view) {
		auto& animation = view.get<AnimationC>(entity);
		auto& sprite = view.get<SpriteC>(entity);
		int distanceToNextSprite = animation.frameSrcs[animation.curFrameSrc].width + ATLAS_SPRITE_PADDING;
		unsigned int atframe = animation.atFrame[animation.curFrameSrc];
		sprite.atlasPos.x = animation.frameSrcs[animation.curFrameSrc].x + atframe * (distanceToNextSprite);
		sprite.atlasPos.y = animation.frameSrcs[animation.curFrameSrc].y;
	}
}

void AnimationSystem::update(entt::entity player)
{
	// test
	auto view = ECS::registry.view<AnimationC, PhysicsC>();

	for (auto entity : view) {
		AnimationC& animation = ECS::registry.get<AnimationC>(entity);
		PhysicsC& physics = ECS::registry.get<PhysicsC>(entity);
		if (entity == player) {
			updatePlayerAnimation(animation, physics);
		}
		else {
			updateNPCAnimations(animation, physics);
		}
	}

	updateSprites();
}
