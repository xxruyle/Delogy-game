#include "npc_system.hpp"
#include "components.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"

void NPCSystem::addNPCs(entt::basic_registry<> &registry)
{
    entt::entity entity = registry.create();
    registry.emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
    registry.emplace<PositionC>(entity, Vector2{(float)GetRandomValue(0, 1000), (float)GetRandomValue(0, 1000)});
    registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 5, 5, Rectangle{0, 0, 16, 16}, false);
    registry.emplace<EmotionC>(entity, GetRandomValue(0, 1));
}

void NPCSystem::update(Scene &scene)
{

    for (int i = 0; i < 20; i++) {
        addNPCs(scene.EntityRegistry);
    }
}

void NPCSystem::moveNPCs(entt::basic_registry<> &registry)
{

    auto view = registry.view<PositionC, EmotionC, PhysicsC>();

    for (auto entity : view) {
        auto &emotionState = view.get<EmotionC>(entity);
        auto &position = view.get<PositionC>(entity);
        auto &physics = view.get<PhysicsC>(entity);

        if (emotionState.emotion == 1) {
            physics.velocity.x = Vector2{1, 0}.x * physics.speed;
        }
        position.pos.x += physics.velocity.x * GetFrameTime();
    }
}
