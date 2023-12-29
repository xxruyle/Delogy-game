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
    RAIL_NE, // top right rail
    RAIL_SW, // bottom left rail
    RAIL_SE, // bottom right rail
    RAIL_V,  // rail vertical
    RAIL_H,  // rail horizontal
};

#define ITEM_NULL                                                                                                      \
    CLITERAL(Item) { NULL_ITEM, 304, 304 }

#define ITEM_RAIL_NW                                                                                                   \
    CLITERAL(Item) { RAIL_NW, 0, 160 } // top left rail
#define ITEM_RAIL_NE                                                                                                   \
    CLITERAL(Item) { RAIL_NE, 32, 160 } // top right rail
#define ITEM_RAIL_SW                                                                                                   \
    CLITERAL(Item) { RAIL_SW, 0, 192 } // bottom left rail
#define ITEM_RAIL_SE                                                                                                   \
    CLITERAL(Item) { RAIL_SE, 32, 192 } // bottom right rail
#define ITEM_RAIL_V                                                                                                    \
    CLITERAL(Item) { RAIL_V, 0, 176 }

#define ITEM_RAIL_H                                                                                                    \
    CLITERAL(Item) { RAIL_H, 16, 160 }

#define NUM_ITEMS 50

static Item itemids[NUM_ITEMS] = {
    ITEM_NULL, ITEM_RAIL_NW, ITEM_RAIL_NE, ITEM_RAIL_SW, ITEM_RAIL_SE, ITEM_RAIL_V, ITEM_RAIL_H,
};
#endif
