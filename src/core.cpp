#include "core.hpp"
#include "eadk.h"

Core::Core() {
    this->running = true;
}

void Core::quit() {
    this->running = false;
}

void Core::loop() {
    uint64_t last_time = eadk_timing_millis(); // Avoid super high delta time
    while (this->running)
    {
        // Calculate the delta time
        uint64_t current_time = eadk_timing_millis();
        int delta = current_time - last_time;
        last_time = current_time;

        // Update all the components
        this->renderer.update();
        this->controls.handle(delta);
        // eadk_timing_msleep(30);
    }
}