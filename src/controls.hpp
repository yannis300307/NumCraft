#ifndef CONTROLS_HPP
#define CONTROLS_HPP

class Core;

class Controls
{
private:
    Core *core;
public:
    Controls (Core *core);

    /*Handle the keyboard inputs.*/
    void handle(int delta);
};

#endif
