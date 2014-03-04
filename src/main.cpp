#include "sgct.h"

#include "core/Engine.h"

void draw();
void preSync();
void postSyncPreDraw();
void init();
void encode();
void decode();
void keyCallback(int key, int action);

sgct::Engine* _engine;

Engine* _gameEngine;


int main(int argc, char *argv[])
{

  _engine = new sgct::Engine(argc, argv);
  _gameEngine = new Engine();

  _engine->setDrawFunction(draw);
  _engine->setPreSyncFunction(preSync);
  _engine->setPostSyncPreDrawFunction(postSyncPreDraw);
  _engine->setInitOGLFunction(init);
  _engine->setKeyboardCallbackFunction(keyCallback);

  sgct::SharedData::instance()->setEncodeFunction(encode);
  sgct::SharedData::instance()->setDecodeFunction(decode);

  if (!_engine->init())
  {
    delete _engine;
    return EXIT_FAILURE;
  }

  _engine->render();

  delete _engine;
  delete _gameEngine;

  exit(EXIT_SUCCESS);


  return 0;
}


void draw()
{

}

void preSync()
{
  double xPos, yPos;
  sgct::Engine::getMousePos(_engine->getFocusedWindowIndex(), &xPos, &yPos);
  _gameEngine->passMousePosition(xPos, yPos);
}

void postSyncPreDraw()
{

}

void init()
{

}

void encode()
{

}

void decode()
{

}

void keyCallback(int key, int action)
{
  if (_engine->isMaster())
    _gameEngine->passToKeyCallback(key, action);
}