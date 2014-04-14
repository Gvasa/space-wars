#include "Input.h"

Input::Input()
{
  for (int i = 0; i < NR_OF_COMMANDS; ++i)
  {
    _commandStates[i] = false;
  }
}

Input::~Input()
{

}

void Input::keyCallback(const int& key, const int& action)
{
  switch(key)
  {
    case 'W':
    {
      _commandStates[FORWARD] = ((action == REPEAT || action == PRESS) ? true : false);   
      break;
    }

    case 'A':
    {
      _commandStates[LEFT] = ((action == REPEAT || action == PRESS) ? true : false);  
      break;
    }

    case 'S':
    {
      _commandStates[BACKWARD] = ((action == REPEAT || action == PRESS) ? true : false);  
      break;
    }

    case 'D':
    {
      _commandStates[RIGHT] = ((action == REPEAT || action == PRESS) ? true : false);  
      break;
    }
  }
}

void Input::setMousePosition(const double& xPos, const double& yPos)
{
  _xPosition = xPos;
  _yPosition = yPos;
}

bool Input::getCommandState(command cmd)
{
  return _commandStates[cmd];
}

double Input::getMousePositionX()
{
  return _xPosition;
}

double Input::getMousePositionY()
{
  return _yPosition;
}

void Input::getMousePosition(double& xPos, double& yPos)
{
  xPos = _xPosition;
  yPos = _yPosition;
}