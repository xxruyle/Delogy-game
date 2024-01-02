#pragma once

enum EntityType { NULL_ENTITY, CART_H, CART_V };

struct Entity {
    int id;
    /* default_atlas.png positions */
    int x;
    int y;
};

#define NUM_ENTITIES 50

#define ENTITY_NULL                                                                                                    \
    CLITERAL(Entity) { NULL_ENTITY, 445, 445 }
#define ENTITY_CART_H                                                                                                  \
    CLITERAL(Entity) { CART_H, 67, 88 }
#define ENTITY_CART_V                                                                                                  \
    CLITERAL(Entity) { CART_V, 88, 88 }

static Entity entityids[NUM_ENTITIES] = {ENTITY_NULL, ENTITY_CART_H, ENTITY_CART_V

};
