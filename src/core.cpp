#include "core.hpp"
#include "eadk.h"

Core::Core()
{
    //world.change_view_distance(2);
    //world.load_chunks_around(0, 0, 0);
    this->update_view_distance(2);

    running = true;
}

void Core::quit()
{
    running = false;
    world.free_all_chunks(); // Super important !! Avoid crashes due to memory leaks.
}

void Core::loop()
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

bool Core::update_view_distance(int view_distance)
{
    //if (!world.change_view_distance(view_distance)) return false;
    //if (!renderer.change_view_distance(view_distance)) return false;
    return true;
}
