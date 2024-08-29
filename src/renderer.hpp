#ifndef RENDERER_HPP
#define RENDERER_HPP

extern "C"
{
#include "libs/eadk.h"
}

class Renderer
{
private:
  int imageCount = 0;

public:
  Renderer();
  void move_camera(int x, int y, int z);
  void move_camera_forward(int step);
  void move_camera_sideway(int step);
  void rotate_camera(int x, int y, int z);
  void set_camera_pos(int x, int y, int z);
  void set_camera_rotation(int x, int y, int z);
  void update();
};

#endif