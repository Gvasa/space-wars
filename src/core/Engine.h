#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "sgct.h"
#include "Physics.h"
#include "Renderer.h"
#include "Input.h"
#include "json/JsonReader.h"

class Engine
{

  public:
    Engine();
    ~Engine();

    void passToKeyCallback(const int& key, const int& action);
    void passMousePosition(const double& xPos, const double& yPos);

    void addObject(std::string filename);

  private:
    Physics* _physics;
    Renderer* _renderer;
    Input* _input;
  
};

#endif 