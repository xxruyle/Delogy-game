#include "atlas.hpp"
#include "components.hpp"
#include "ui_util.hpp"

enum BoundType { HOTBAR, MINIMAP };

class UI {
public:
  Rectangle bounds[2];

  bool mouseOutOfBounds(); // check if mouse is not in the ui bound space
  const float gridSpacing = 2.0f;
  void hotBar(Atlas &atlas, InventoryC &inventory, HotBarC &hotBar, int width,
              int height);
  void inventory(Atlas &atlas, InventoryC &inventory, HotBarC &hotBar,
                 int width, int height, int cellsPerRow);
  void miniMap(Rectangle mapSrc);
};
