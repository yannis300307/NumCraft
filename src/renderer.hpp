#ifndef renderer_hpp
#define renderer_hpp

extern "C" {
  #include "libs/eadk.h"
}


class Renderer
{
private:
    int imageCount = 0;
public:
    Renderer ();
    void move_camera(int x, int y, int z);
    void rotate_camera(int x, int y, int z);
    void set_camera_pos(int x, int y, int z);
    void set_camera_rotation(int x, int y, int z);
    void update();
};

#endif