#ifndef INPUT_H
#define INPUT_H

#include <iostream>

#include "sgct.h"

class Input
{

  public:
    enum command {FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3};
    static const int NR_OF_COMMANDS = 4;
    
    Input();
    ~Input();

    void keyCallback(const int& key, const int& action);
    void setMousePosition(const double& xPos, const double& yPos);
    
    bool getCommandState(enum command cmd);
    double getMousePositionX();
    double getMousePositionY();
    void getMousePosition(double& xPos, double& yPos);

  private:
    bool _commandStates[NR_OF_COMMANDS];
    double _xPosition = 0, _yPosition = 0;
};

#endif