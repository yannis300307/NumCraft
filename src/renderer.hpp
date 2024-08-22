#ifndef renderer_hpp
#define renderer_hpp

extern "C" {
  #include "libs/eadk.h"
}

#define S3L_PIXEL_FUNCTION draw_pixel
#define S3L_RESOLUTION_X 340
#define S3L_RESOLUTION_Y 240

#include "libs/small3dlib.h"

#include "core.hpp"

S3L_Unit cubeVertices[] = { S3L_CUBE_VERTICES(S3L_F) };
S3L_Index cubeTriangles[] = { S3L_CUBE_TRIANGLES };

S3L_Model3D cubeModel;
S3L_Scene scene;

void draw_pixel(S3L_PixelInfo *pixel);

class Core;

class Renderer
{
private:
    S3L_Scene scene;
    Core *core;
    int imageCount = 0;
public:
    Renderer (Core *core);
    void update();
};

#endif