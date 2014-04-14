#include "Engine.h"

Engine::Engine()
{
  _physics = new Physics();
  _renderer = new Renderer();
  _input = new Input();
}

Engine::~Engine()
{
  delete _physics;
  delete _renderer;
  delete _input;
}

void Engine::passToKeyCallback(const int& key, const int& action)
{
 _input->keyCallback(key, action);
}

void Engine::passMousePosition(const double& xPos, const double& yPos)
{
  _input->setMousePosition(xPos, yPos);
}

void Engine::addObject(std::string filename)
{
  JsonValue value;
  JsonParseStatus status = parseJsonFile(filename, value);
  
  if (status == JSON_PARSE_OK)
  {
    // print(value);
    switch (value.getTag())
    {
      case JSON_TAG_STRING:
        std::cout << "dafsffff" << value.toString() << std::endl;
      break;
        
      
    }
  }else
    sgct::MessageHandler::instance()->print("Failed to load object from json file");

}