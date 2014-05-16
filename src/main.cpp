#include <iostream>
#include <list>

#include "sgct.h"
#include <btBulletDynamicsCommon.h>
#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/RigidBody.h>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/computeBoundsVisitor>

#include "core/Physics.h"
#include "core/Renderer.h"
#include "core/Input.h"
#include "core/Info.h"
#include "objects/AssetsLibrary.h"
#include "objects/GameObject.h"
#include "objects/BulletObject.h"
#include "objects/PlayerObject.h"


void draw();
void preSync();
void postSyncPreDraw();
void init();
void encode();
void decode();

sgct::Engine* _engine;

Renderer* _renderer;
Physics* _physics;
Input* _input;

sgct::SharedDouble _currentTime(0.0);
sgct::SharedObject<glm::mat4> _sceneTransform;

std::list<BulletObject*> _bullets;
std::list<PlayerObject*> _players;

int main(int argc, char *argv[])
{
  AssetsLibrary::init();
  _engine = new sgct::Engine(argc, argv);

  _renderer = new Renderer();
  _input = new Input();
  _physics = new Physics(_input, &_bullets);

  _engine->setDrawFunction(draw);
  _engine->setPreSyncFunction(preSync);
  _engine->setPostSyncPreDrawFunction(postSyncPreDraw);
  _engine->setInitOGLFunction(init);
  _engine->setKeyboardCallbackFunction([](int key, int action){
    if (_engine->isMaster())
      _input->keyCallback(key, action);
  });
  _engine->setMouseButtonCallbackFunction([](int key, int action){
    if (_engine->isMaster())
      _input->mouseCallback(key, action);
  });

  sgct::SharedData::instance()->setEncodeFunction(encode);
  sgct::SharedData::instance()->setDecodeFunction(decode);

  if (!_engine->init())
  {
    delete _engine;
    return EXIT_FAILURE;
  }
  sgct::Engine::setMousePos(_engine->getFocusedWindowIndex(), _engine->getActiveXResolution()/2.0f, _engine->getActiveYResolution()/2.0f);
  _engine->setDisplayInfoVisibility(false);
  _engine->setMouseCursorVisibility(_engine->getFocusedWindowIndex(), false);
  _engine->getActiveWindowPtr()->setNumberOfAASamples(16);
  _engine->setNearAndFarClippingPlanes(0.1,5000);
  std::cout << "Clipping planes: " << _engine->getNearClippingPlane() << " " << _engine->getFarClippingPlane() << std::endl;


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
    Info::setXresolution(resolution[0]);
    Info::setYresolution(resolution[1]);
    _physics->updatePreSync(mousePos, resolution, sgct::Engine::getUserPtr()->getPos(), _engine->getDt());
    _sceneTransform.setVal(_physics->getPlayerTransform());

    _renderer->updatePreSync(_currentTime.getVal(), mousePos);

    Info::setPlayerPosition(sgct::Engine::getUserPtr()->getPos());
  }
  
}

void postSyncPreDraw()
{ 
  _renderer->setTranslationTransform(_physics->getTranslationMatrix());
  _renderer->setRotationTransform(_physics->getRotationMatrix());

  if (_input->getCommandState(_input->FIRE))
  {
    // std::cout << "hej" << std::endl;
    _bullets.push_back(new BulletObject(AssetsLibrary::getCollisionShape(AssetsLibrary::BULLET), AssetsLibrary::getNode(AssetsLibrary::BULLET), glm::translate(glm::mat4(1.0f), glm::vec3(0)), glm::vec3(10,1,1)));
    // _dynamicsWorld->addRigidBody(_bulletList.back()->getRigidBody());
    
    _renderer->addNodeToScene(_bullets.back()->getNode());
    _physics->addCollisionShape(_bullets.back()->getCollisionShape(),_players.back()->getBulletTransform());

  }
  _physics->updatePostSync(_engine->getDt());
  _renderer->updatePostSync(_currentTime.getVal(), _engine->getCurrentFrameNumber(), _engine->getModelMatrix());

  for (int i = 0; i < _physics->getNumberOfRigidBodies(); ++i)
  {
    _renderer->updateNode(i, _physics->getRigidBodyTransform(i));
  }
}

void init()
{

  _players.push_back(new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::FIGHTER), AssetsLibrary::getNode(AssetsLibrary::FIGHTER), glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))));
  _physics->registerPlayerObject(_players.back());
  _renderer->registerPlayerObject(_players.back());

  _renderer->addNodeToScene(AssetsLibrary::getNode(AssetsLibrary::FIGHTER));
  _physics->addCollisionShape(AssetsLibrary::getCollisionShape(AssetsLibrary::FIGHTER), glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
  
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
