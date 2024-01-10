#include "draw_system.hpp"
#include "components.hpp"
#include "raylib.h"
#include <iostream>

void SpriteDrawSystem::drawSprites(Texture2D &atlas, entt::basic_registry<> &registry)
{

    auto view = registry.view<SpriteC, PositionC>();

    for (auto entity : view) {
        auto &sprite = view.get<SpriteC>(entity);
        auto &position = view.get<PositionC>(entity);

        DrawTexturePro(atlas, sprite.atlasPos, Rectangle{position.pos.x, position.pos.y, 16, 16}, {0, 0}, 0.0f, WHITE);
    }
}
