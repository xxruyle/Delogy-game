#pragma once

struct Item {
  int id;
  int x;
  int y;
};

enum ItemType {
  NULL_ITEM,
  RAIL_NW, // top left rail 1
  RAIL_V,  // rail vertical
  RAIL_NE, // top right rail
  RAIL_H,  // rail horizontal
  RAIL_SE, // bottom right rail
  RAIL_SW, // bottom left rail 6
  STORAGE_BOX,
  CART, // they have underscores because they are items ;)
};

#define ITEM_NULL                                                              \
  (Item) { NULL_ITEM, 304, 304 }

#define ITEM_RAIL_NW                                                           \
  (Item) { RAIL_NW, 4, 88 } // top left rail
#define ITEM_RAIL_NE                                                           \
  (Item) { RAIL_NE, 46, 88 } // top right rail
#define ITEM_RAIL_SW                                                           \
  (Item) { RAIL_SW, 4, 130 } // bottom left rail
#define ITEM_RAIL_SE                                                           \
  (Item) { RAIL_SE, 46, 130 } // bottom right rail
#define ITEM_RAIL_V                                                            \
  (Item) { RAIL_V, 4, 109 }

#define ITEM_RAIL_H                                                            \
  (Item) { RAIL_H, 25, 88 }

#define ITEM_STORAGE_BOX                                                       \
  (Item) { STORAGE_BOX, 67, 130 }

#define ITEM_CART                                                              \
  (Item) { CART, 67, 88 }

#define NUM_ITEMS 50

static Item itemids[NUM_ITEMS] = {ITEM_NULL,    ITEM_RAIL_NW,     ITEM_RAIL_V,
                                  ITEM_RAIL_NE, ITEM_RAIL_H,      ITEM_RAIL_SE,
                                  ITEM_RAIL_SW, ITEM_STORAGE_BOX, ITEM_CART};
