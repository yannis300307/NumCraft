#ifndef CORE_HPP
#define CORE_HPP

#include "renderer.hpp"
#include "controls.hpp"
#include "world.hpp"

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
    World world;
};

#endif