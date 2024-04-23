#include "npc_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "tile_manager.hpp"

NPCSystem::NPCSystem(TileManager* tileManager) { tManager = tileManager; }

void NPCSystem::addNPCs(entt::basic_registry<>& registry)
{
    for (int i = 0; i < 4; i++) {
        entt::entity entity = registry.create();
        registry.emplace<SpriteC>(entity, AtlasType::SMALL, Rectangle{88, 4, 16, 16});
        registry.emplace<PositionC>(entity, Vector2{1, 1});
        registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, 5, 5, false);
        registry.emplace<CollisionC>(entity, Rectangle{0, 0, 16, 16});
        registry.emplace<NeedsC>(entity, NeedsC{{(float)GetRandomValue(0, 3) / 10.0f, 0.3f, 0.1f, 0.1f, 0.3f}});
    }
}

void NPCSystem::update(Scene& scene)
{

    for (int i = 0; i < 20; i++) {
        addNPCs(scene.EntityRegistry);
    }
}

void NPCSystem::cachePosition(Vector2 pos, entt::entity id)
{

    /* std::cout << getVector2String(pos) << std::endl; */

    tManager->entityPositionCache[pos].push_back(id);
}

void NPCSystem::moveNPCs(entt::basic_registry<>& registry)
{

    auto view = registry.view<PositionC, NeedsC, PhysicsC>();

    for (auto entity : view) {
        auto& needs = view.get<NeedsC>(entity);
        auto& position = view.get<PositionC>(entity);

        Vector2 entityPos = getGridPosition(position.pos);
        cachePosition(entityPos, entity);

        auto& physics = view.get<PhysicsC>(entity);
    }
}
