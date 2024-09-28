#include "keybindings.hpp"

namespace Keybindings {
int binds[NUM_KEYS] = {
    KEY_NULL,
    KEY_D,
    KEY_A,
    KEY_W,
    KEY_S,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    KEY_I,
    KEY_E,
    KEY_R,
    KEY_M,
    KEY_LEFT_SHIFT,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_LEFT,
    KEY_C
};

void rebind(BindT old_key, int new_bind) {
    binds[old_key] = new_bind; 
}

}