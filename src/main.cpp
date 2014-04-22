#include "sgct.h"

#include "core/Physics.h"
#include "core/Renderer.h"
#include "core/Input.h"

#include <iostream>

void draw();
void preSync();
void postSyncPreDraw();
void init();
void encode();
void decode();
void keyCallback(int key, int action);

sgct::Engine* _engine;

Renderer* _renderer;
Physics* _physics;
Input* _input;

sgct::SharedDouble _currentTime(0.0);
sgct::SharedObject<glm::mat4> _sceneTransform;

int main(int argc, char *argv[])
{

  _engine = new sgct::Engine(argc, argv);

  _renderer = new Renderer();
  _input = new Input();
  _physics = new Physics(_input);

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

  _engine->getActiveWindowPtr()->setNumberOfAASamples(16);

  _engine->render();

  delete _engine;
  delete _physics;
  delete _renderer;
  delete _input;

  exit(EXIT_SUCCESS);

  return 0;
}


void draw()
{
  const int* curr_vp = _engine->getActiveViewportPixelCoords();
  _renderer->setPixelCoords(curr_vp[0], curr_vp[1], curr_vp[2], curr_vp[3]);
  _renderer->setProjectionMatrix(_engine->getActiveViewProjectionMatrix());
  _renderer->render();
}

void preSync()
{
  

  if (_engine->isMaster())
  {
    _currentTime.setVal(sgct::Engine::getTime());
    double xPos, yPos;
    sgct::Engine::getMousePos(_engine->getFocusedWindowIndex(), &xPos, &yPos);
    _input->setMousePosition(xPos, yPos);

    int mousePos[2];
    int resolution[2];
    mousePos[0] = xPos;
    mousePos[1] = yPos;
    resolution[0] = _engine->getActiveXResolution();
    resolution[1] = _engine->getActiveYResolution();
    _physics->updatePreSync(mousePos, resolution, sgct::Engine::getUserPtr()->getPos(), _engine->getDt());
    _sceneTransform.setVal(_physics->getPlayerTransform());
  }
  
}

void postSyncPreDraw()
{
  _renderer->setSceneTransform(_sceneTransform.getVal());

  _physics->updatePostSync(_engine->getDt());
  _renderer->updatePostSync(_currentTime.getVal(), _engine->getCurrentFrameNumber(), _engine->getModelMatrix());

}

void init()
{

}

void encode()
{
  sgct::SharedData::instance()->writeDouble(&_currentTime);
  sgct::SharedData::instance()->writeObj(&_sceneTransform);
}

void decode()
{
  sgct::SharedData::instance()->readDouble(&_currentTime);
  sgct::SharedData::instance()->readObj(&_sceneTransform);
}

void keyCallback(int key, int action)
{
  if (_engine->isMaster())
    _input->keyCallback(key, action);
}