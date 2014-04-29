#include <iostream>

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
  sgct::Engine::setMousePos(_engine->getFocusedWindowIndex(), _engine->getActiveXResolution()/2.0f, _engine->getActiveYResolution()/2.0f);
  _engine->setDisplayInfoVisibility(true);
  _engine->setMouseCursorVisibility(_engine->getFocusedWindowIndex(), false);
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
    Info::setXresolution(resolution[0]);
    Info::setYresolution(resolution[1]);
    _physics->updatePreSync(mousePos, resolution, sgct::Engine::getUserPtr()->getPos(), _engine->getDt());
    _sceneTransform.setVal(_physics->getPlayerTransform());

    _renderer->updatePreSync(_currentTime.getVal(), mousePos);
  }
  
}

void postSyncPreDraw()
{
  _renderer->setSceneTransform(_sceneTransform.getVal());

  _physics->updatePostSync(_engine->getDt());
  _renderer->updatePostSync(_currentTime.getVal(), _engine->getCurrentFrameNumber(), _engine->getModelMatrix());

  for (int i = 0; i < _physics->getNumberOfRigidBodies(); ++i)
  {
    _renderer->updateNode(i, _physics->getRigidBodyTransform(i));
  }
}

void init()
{
  osg::ref_ptr<osg::Node> astroidNode;
  astroidNode = osgDB::readNodeFile("assets/models/fighter.obj");

  // osg::ref_ptr<osg::Node> fighterNode;
  // fighterNode = osgDB::readNodeFile("assets/models/fighter.obj");

  if (astroidNode.valid())
  {
    btCollisionShape* asteroidShape = osgbCollision::btConvexTriMeshCollisionShapeFromOSG(astroidNode);

    // btCollisionShape* fighterShape = osgbCollision::btConvexTriMeshCollisionShapeFromOSG(fighterNode);

     // _renderer->addNodeToScene(osgbCollision::osgNodeFromBtCollisionShape(asteroidShape));

    // astroidTrans->addChild(astroidNode.get());
    // _renderer->addNodeToScene(astroidNode.get());
    // _physics->addCollisionShape(asteroidShape, glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

    // _renderer->addNodeToScene(fighterNode.get());
    // _physics->addCollisionShape(fighterShape, glm::translate( glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)));
  }
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
