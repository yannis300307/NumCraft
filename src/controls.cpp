#include "controls.hpp"

extern "C"
{
  #include "eadk.h"
}

#include "core.hpp"

Controls::Controls(Core *core)
{
  this->core = core;
}

void Controls::handle()
{
  eadk_keyboard_state_t keyboardState = eadk_keyboard_scan();

  // Technical
  if (eadk_keyboard_key_down(keyboardState, eadk_key_home))
  {
    this->core->quit();
  }

  // Y axis
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_backspace))
  {
    this->core->renderer.move_camera(0, 100, 0);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_power))
  {
    this->core->renderer.move_camera(0, -100, 0);
  }

  // X Z axis
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_var))
  {
    this->core->renderer.move_camera(0, 0, 100);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_imaginary))
  {
    this->core->renderer.move_camera(0, 0, -100);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_comma))
  {
    this->core->renderer.move_camera(100, 0, 0);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_log))
  {
    this->core->renderer.move_camera(-100, 0, 0);
  }

  // Rotation
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_up))
  {
    this->core->renderer.rotate_camera(10, 0, 0);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_down))
  {
    this->core->renderer.rotate_camera(-10, 0, 0);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_right))
  {
    this->core->renderer.rotate_camera(0, -10, 0);
  }
  else if (eadk_keyboard_key_down(keyboardState, eadk_key_left))
  {
    this->core->renderer.rotate_camera(0, 10, 0);
  }
}