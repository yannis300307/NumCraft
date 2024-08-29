#include "core.hpp"
#include "eadk.h"

Core::Core()
{
    running = true;
}

void Core::quit()
/*Free the memory and close the app.*/
{
    running = false;
    world.free_all_chunks(); // Super important !! Avoid crashes due to memory leaks.
}

void Core::loop()
/*The main loop of the program.*/
{
    uint64_t last_time = eadk_timing_millis(); // Avoid super high delta time
    while (running)
    {
        // Calculate the delta time
        uint64_t current_time = eadk_timing_millis();
        int delta = current_time - last_time;
        last_time = current_time;

        // Update all the components
        renderer.update();
        controls.handle(delta);
        // eadk_timing_msleep(30);
    }
}