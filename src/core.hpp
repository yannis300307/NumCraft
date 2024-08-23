#ifndef core_hpp
#define core_hpp

#include "renderer.hpp"
#include "controls.hpp"

class Renderer;
class Controls;

class Core
{
private:
    bool running;
public:
    Core();
    void quit();
    void loop();
    Renderer renderer;
    Controls controls = Controls(this);
};

#endif