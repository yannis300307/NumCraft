#ifndef CONTROLS_HPP
#define CONTROLS_HPP

class Core;

class Controls
{
private:
    Core *core;
public:
    Controls (Core *core);
    void handle(int delta);
};

#endif
