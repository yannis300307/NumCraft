#ifndef RENDERER_HPP
#define RENDERER_HPP

extern "C"
{
#include "libs/eadk.h"
}

class Core;

class Renderer
{
private:
  int imageCount = 0;
  Core *core;

public:
  Renderer();

  /*Move the absolute position of the camera.*/
  void move_camera(int x, int y, int z);

  /*Move the camera relatively to the camera rotation forward. Positive step is forward and negative step is backward.*/
  void move_camera_forward(int step);

  /*Move the camera relatively to the camera rotation sideway. Positive step is to the right and negative step is to the left.*/
  void move_camera_sideway(int step);

  /*Rotate the camera.*/
  void rotate_camera(int x, int y, int z);

  /*Set the absolute position of the camera.*/
  void set_camera_pos(int x, int y, int z);

  /*Set the rotation of the camera.*/
  void set_camera_rotation(int x, int y, int z);

  /*Change the view distance and realocate chunk models.*/
  bool change_view_distance(int view_distance);

  /*Render the scene.*/
  void update();
};

#endif