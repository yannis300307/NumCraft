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

    /*Free the memory and close the app.*/
    void quit();

    /*The main loop of the program.*/
    void loop();

    // Components
    
    Renderer renderer;
    Controls controls = Controls(this);
    World world;
};

#endif