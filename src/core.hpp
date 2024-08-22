#ifndef core_hpp
#define core_hpp

#include "renderer.hpp"

class Renderer;

class Core
{
private:
    bool running;
public:
    Core();
    void quit();
    void loop();
    Renderer *renderer;
};

#endif