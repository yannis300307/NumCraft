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

    /*Update the view distance of the componenent. If the view distance is too high and the memory needed to store all the chunks is too high, it return false. Otherways, it returns true.*/
    bool update_view_distance(int view_distance);

    // Components
    
    Renderer renderer;
    Controls controls = Controls(this);
    World world;
};

#endif