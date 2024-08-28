#include "controls.hpp"

extern "C"
{
#include "eadk.h"
}

#include "core.hpp"

#define CAMERA_SPEED 2

Controls::Controls(Core *core)
{
  this->core = core;
}

void Controls::handle(int delta)
{
  eadk_keyboard_state_t keyboardState = eadk_keyboard_scan();

  // Technical
  if (eadk_keyboard_key_down(keyboardState, eadk_key_home))               // Close the game
  {
    this->core->quit();
  }

  // Y axis
  if (eadk_keyboard_key_down(keyboardState, eadk_key_backspace))          // Fly higher
  {
    this->core->renderer.move_camera(0, 100, 0);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_power))              // Fly lower
  {
    this->core->renderer.move_camera(0, -100, 0);
  }

  // X Z axis
  if (eadk_keyboard_key_down(keyboardState, eadk_key_var))                // Walk forward
  {
    this->core->renderer.move_camera_forward(CAMERA_SPEED * delta);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_imaginary))          // Walk backward
  {
    this->core->renderer.move_camera_forward(-CAMERA_SPEED * delta);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_comma))              // Walk to the right
  {
    this->core->renderer.move_camera_sideway(CAMERA_SPEED * delta);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_log))                // Walk to the left
  {
    this->core->renderer.move_camera_sideway(-CAMERA_SPEED * delta);
  }

  // Rotation
  if (eadk_keyboard_key_down(keyboardState, eadk_key_up))
  {
    this->core->renderer.rotate_camera(10, 0, 0);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_down))
  {
    this->core->renderer.rotate_camera(-10, 0, 0);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_right))
  {
    this->core->renderer.rotate_camera(0, -10, 0);
  }
  if (eadk_keyboard_key_down(keyboardState, eadk_key_left))
  {
    this->core->renderer.rotate_camera(0, 10, 0);
  }
}