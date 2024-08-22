#include "controls.hpp"


Controls::Controls(Core *core)
{
    this->core = core;
}

void Controls::handle() {
    eadk_keyboard_state_t keyboardState = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(keyboardState, eadk_key_home)) {
      this->core->quit();
    }
}