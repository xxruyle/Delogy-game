#include "draw_system.hpp"
#include "components.hpp"
#include "raylib.h"
#include <iostream>

void SpriteDrawSystem::drawSprites(Atlas &atlas, entt::basic_registry<> &registry)
{

    auto view = registry.view<SpriteC, PositionC>();

    for (auto entity : view) {
        auto &sprite = view.get<SpriteC>(entity);
        auto &position = view.get<PositionC>(entity);
        /* std::cout << position.pos.x << " " << position.pos.y << std::endl; */

        if (sprite.atlasID == AtlasType::SMALL) {

            DrawTexturePro(smallAtlas.texture, sprite.atlasPos,
                           Rectangle{position.pos.x, position.pos.y, smallAtlas.width, smallAtlas.height}, {0, 0}, 0.0f,
                           WHITE);
        }
        else if (sprite.atlasID == AtlasType::MEDIUM) {
            DrawTexturePro(mediumAtlas.texture, sprite.atlasPos,
                           Rectangle{position.pos.x, position.pos.y, 32.0f, 32.0f}, {0, 0}, 0.0f, WHITE);
        }
    }
}
