#include "core.hpp"
#ifndef controls_hpp
#define controls_hpp

class Controls
{
private:
    Core *core;
public:
    Controls(Core *core);
    void handle();
};

#endif
