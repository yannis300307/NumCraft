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
    void update();
};

#endif