#ifndef ITEM_DATA_HPP_
#define ITEM_DATA_HPP_

struct Item {
    int id;
    int x;
    int y;
};

enum ItemType {
    NULL_ITEM,
    RAIL_NW, // top left rail
    RAIL_V,  // rail vertical
    RAIL_NE, // top right rail
    RAIL_H,  // rail horizontal
    RAIL_SE, // bottom right rail
    RAIL_SW, // bottom left rail
    CART_H_, // they have underscores because they are items ;)
    CART_V_,
};

#define ITEM_NULL                                                                                                      \
    CLITERAL(Item) { NULL_ITEM, 304, 304 }

#define ITEM_RAIL_NW                                                                                                   \
    CLITERAL(Item) { RAIL_NW, 4, 88 } // top left rail
#define ITEM_RAIL_NE                                                                                                   \
    CLITERAL(Item) { RAIL_NE, 46, 88 } // top right rail
#define ITEM_RAIL_SW                                                                                                   \
    CLITERAL(Item) { RAIL_SW, 4, 130 } // bottom left rail
#define ITEM_RAIL_SE                                                                                                   \
    CLITERAL(Item) { RAIL_SE, 46, 130 } // bottom right rail
#define ITEM_RAIL_V                                                                                                    \
    CLITERAL(Item) { RAIL_V, 4, 109 }

#define ITEM_RAIL_H                                                                                                    \
    CLITERAL(Item) { RAIL_H, 25, 88 }

#define ITEM_CART_H                                                                                                    \
    CLITERAL(Item) { CART_H_, 67, 88 }
#define ITEM_CART_V                                                                                                    \
    CLITERAL(Item) { CART_V_, 88, 88 }

#define NUM_ITEMS 50

static Item itemids[NUM_ITEMS] = {ITEM_NULL,    ITEM_RAIL_NW, ITEM_RAIL_V, ITEM_RAIL_NE, ITEM_RAIL_H,
                                  ITEM_RAIL_SE, ITEM_RAIL_SW, ITEM_CART_H, ITEM_CART_V};
#endif