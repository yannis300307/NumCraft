#include "core.hpp"

Core::Core() {
    this->running = true;
}

void Core::quit() {
    this->running = false;
}

void Core::loop() {
    while (this->running)
    {
       this->renderer.update();
       eadk_timing_msleep(100);
    }
    
}