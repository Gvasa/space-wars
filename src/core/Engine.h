#ifndef ENGINE_H
#define ENGINE_H

#include "Physics.h"
#include "Renderer.h"
#include "Input.h"



class Engine
{

  public:
    Engine();
    ~Engine();

    void passToKeyCallback(const int& key, const int& action);
    void passMousePosition(const double& xPos, const double& yPos);

  private:
    Physics* _physics;
    Renderer* _renderer;
    Input* _input;
  
};

#endif 