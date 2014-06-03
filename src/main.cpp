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
sgct::SharedDouble mousePosX(0.0);
sgct::SharedDouble mousePosY(0.0);
sgct::SharedInt resolutionX(0);
sgct::SharedInt resolutionY(0);
bool commadStorage[Input::NR_OF_COMMANDS];
sgct::SharedBool commands[Input::NR_OF_COMMANDS];
sgct::SharedObject<glm::mat4> _sceneTransform;

std::list<BulletObject*> _bullets;
std::list<PlayerObject*> _players;

int main(int argc, char *argv[])
{
  AssetsLibrary::init();
  _engine = new sgct::Engine(argc, argv);

  _input = new Input();
  _renderer = new Renderer(&_bullets);
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

  int numberOfNodes = sgct_core::ClusterManager::instance()->getNumberOfNodes();
  std::cout << "Number of Nodes: " << numberOfNodes << std::endl;
  int nodeNumber = sgct_core::ClusterManager::instance()->getThisNodeId();
  std::cout << "I am number " << nodeNumber << std::endl;
  if ( numberOfNodes > 1){
    if ( nodeNumber == 1)
      _renderer->addGui();
  }else
  {
    _renderer->addGui();
  }
  

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
    mousePosX.setVal(xPos);
    mousePosY.setVal(yPos);

    int mousePos[2];
    int resolution[2];
    mousePos[0] = xPos;
    mousePos[1] = yPos;
    resolution[0] = _engine->getActiveXResolution();
    resolution[1] = _engine->getActiveYResolution();
    resolutionX.setVal(_engine->getActiveXResolution());
    resolutionY.setVal(_engine->getActiveYResolution());
    
    _physics->updatePreSync(mousePos, resolution, sgct::Engine::getUserPtr()->getPos(), _engine->getDt());
    _sceneTransform.setVal(_physics->getPlayerTransform());

    _renderer->updatePreSync(_currentTime.getVal(), mousePos);

    _input->getCommandsAsArray(commadStorage);
    for (int i = 0; i < Input::NR_OF_COMMANDS; i++)
      commands[i].setVal(commadStorage[i]);
  }
  
}

float cooldown = 0.2f;
bool rightGun = false;
void postSyncPreDraw()
{ 
  
  Info::setPlayerPosition(sgct::Engine::getUserPtr()->getPos());

  for (int i = 0; i < Input::NR_OF_COMMANDS; i++)
    commadStorage[i] = commands[i].getVal();

  _input->setCommadsFromArray(commadStorage);

  _input->setMousePosition(mousePosX.getVal(), mousePosY.getVal());
  Info::setXresolution(resolutionX.getVal());
  Info::setYresolution(resolutionY.getVal());
  _renderer->setTranslationTransform(_physics->getTranslationMatrix());
  _renderer->setRotationTransform(_physics->getRotationMatrix());

  cooldown -= _engine->getDt();

  if (cooldown <= 0 && _input->getCommandState(_input->FIRE))
  { 
    std::cout << "---------------------" << std::endl;
    cooldown = 0.2f;


    glm::vec4 playerDirection = glm::vec4(0,0,10,0) * _players.back()->getRotationMatrix(); 
    glm::mat4 bulletTransform;
   
   float ofset = 0;

    // if(rightGun) 
    // {
    //   ofset = 2.0f;
    //   rightGun = false;
    // }
    // else
    // {
    //   ofset = -2.0f;
    //   rightGun = true;
    // }

    glm::vec3 playerPos = Info::getPlayerPosition();

    // bulletTransform = glm::translate(glm::mat4(1), glm::vec3(playerDirection.x+ofset, playerDirection.y+0.5, playerDirection.z)) *  _players.back()->getTranslationMatrix() * _players.back()->getRotationMatrix();

    
    bulletTransform =  _players.back()->getTranslationMatrix() * glm::translate(glm::mat4(1), glm::vec3(playerDirection.x+ofset, playerDirection.y+0.2, playerDirection.z)); 
    // * _players.back()->getRotationMatrix() * glm::translate( glm::mat4(1.0f), playerPos) * _players.back()->getTranslationMatrix()
    // bulletTransform = glm::transpose(bulletTransform);
    // _players.back()->getTranslationMatrix() * glm::translate( glm::mat4(1.0f), playerPos);

  
    // std::cout << "Determinatn: "  << glm::determinant(bulletTransform) << std::endl;
    // swutils::printMat4(bulletTransform);
    BulletObject* bt = new BulletObject(AssetsLibrary::getCollisionShape(AssetsLibrary::BULLET), AssetsLibrary::getNode(AssetsLibrary::BULLET),bulletTransform , glm::vec3(0,0,0));

   std::cout << "Number of obejcts" << GameObject::getNumberOfObjects() << std::endl;
    _bullets.push_back(bt); 

    
    
    // _renderer->addNodeToScene(_bullets.back()->getNode());
    _renderer->addObject(_bullets.back());
    _physics->addObject(_bullets.back());

    std::cout << "---------------------" << std::endl;

  }

  GameObject::updateAllObjects();

  _physics->updatePostSync(_engine->getDt());
  _renderer->updatePostSync(_currentTime.getVal(), _engine->getCurrentFrameNumber(), _engine->getModelMatrix());

}

void init()
{


  _players.push_back(new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::FIGHTER), AssetsLibrary::getNode(AssetsLibrary::FIGHTER), glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f))));
  _physics->registerPlayerObject(_players.back());
  _renderer->registerPlayerObject(_players.back());

  // Adding some ships and asteroids
  PlayerObject* as1 = new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::ASTEROID), AssetsLibrary::getNode(AssetsLibrary::ASTEROID), glm::translate( glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 20.0f)));
  _physics->addObject(as1);
  _renderer->addObject(as1);

  PlayerObject* as2 = new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::ASTEROID), AssetsLibrary::getNode(AssetsLibrary::ASTEROID), glm::translate( glm::mat4(1.0f), glm::vec3(12.0f,5.0f, 19.0f)));
  _physics->addObject(as2);
  _renderer->addObject(as2);

  PlayerObject* as3 = new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::ASTEROID), AssetsLibrary::getNode(AssetsLibrary::ASTEROID), glm::translate( glm::mat4(1.0f), glm::vec3(9.0f, -1.0f, 22.0f)));
  _physics->addObject(as3);
  _renderer->addObject(as3);

  PlayerObject* as4 = new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::ASTEROID), AssetsLibrary::getNode(AssetsLibrary::ASTEROID), glm::translate( glm::mat4(1.0f), glm::vec3(7.0f, 2.0f, 15.0f)));
  _physics->addObject(as4);
  _renderer->addObject(as4);


  glm::mat4 hej =  glm::rotate(glm::mat4(1), 1.5f, glm::vec3(1.0f, 0.0f, 0.0f));

  PlayerObject* sh1 = new PlayerObject(AssetsLibrary::getCollisionShape(AssetsLibrary::FIGHTER), AssetsLibrary::getNode(AssetsLibrary::FIGHTER), glm::translate( glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 20.0f)) * hej);
  _physics->addObject(sh1);
  _renderer->addObject(sh1);

  
}

void encode()
{
  sgct::SharedData::instance()->writeDouble(&_currentTime);
  sgct::SharedData::instance()->writeDouble(&mousePosX);
  sgct::SharedData::instance()->writeDouble(&mousePosY);
  sgct::SharedData::instance()->writeInt(&resolutionX);
  sgct::SharedData::instance()->writeInt(&resolutionY);
  for (int i = 0; i < Input::NR_OF_COMMANDS; i++)
    sgct::SharedData::instance()->writeBool(&commands[i]);

  sgct::SharedData::instance()->writeObj(&_sceneTransform);
}

void decode()
{
  sgct::SharedData::instance()->readDouble(&_currentTime);
  sgct::SharedData::instance()->readDouble(&mousePosX);
  sgct::SharedData::instance()->readDouble(&mousePosY);
  sgct::SharedData::instance()->readInt(&resolutionX);
  sgct::SharedData::instance()->readInt(&resolutionY);
  for (int i = 0; i < Input::NR_OF_COMMANDS; i++)
    sgct::SharedData::instance()->readBool(&commands[i]);
  sgct::SharedData::instance()->readObj(&_sceneTransform);
}
  