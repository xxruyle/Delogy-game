#include "atlas.hpp"
#include "components.hpp"
#include "ui_util.hpp"

enum BoundType { HOTBAR, MINIMAP };

class UI {
public:
  Rectangle bounds[2];

  bool mouseOutOfBounds(); // check if mouse is not in the ui bound space
  const float gridSpacing = 2.0f;
  void hotBar(Atlas &atlas, InventoryC &inventory);
  void miniMap(Rectangle mapSrc);
};
