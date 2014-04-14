#include "Physics.h"

Physics::Physics(Input* input)
{
  _input = input;
  _broadphase = new btDbvtBroadphase();
  _collisionConfiguration = new btDefaultCollisionConfiguration();
  _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
  _solver = new btSequentialImpulseConstraintSolver();
  _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);

  _playerShape = new btSphereShape(1);
  _playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)));
  _playerRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(10, _playerMotionState, _playerShape, btVector3(0,0,0)));

  _dynamicsWorld->addRigidBody(_playerRigidBody);
}

Physics::~Physics()
{
  delete _dynamicsWorld;
  delete _solver;
  delete _collisionConfiguration;
  delete _dispatcher;
  delete _broadphase;
}

void Physics::updatePreSync(int* mousePos, int* resolution, glm::vec3 playerPos)
{
  double mouseDx = mousePos[0] - resolution[0]/2.0f;
  double mouseDy = mousePos[1] - resolution[1]/2.0f;

  if(abs(mouseDx) < 20){      
      mouseDx = 0;      
  } 
  else if(abs(mouseDx) > 400){
    mouseDx = (mouseDx < 0) ? 1 : -1;
  }
  else
    mouseDx = mouseDx/400*-1;

  if(abs(mouseDy) < 20)
    mouseDy = 0;
  else if(abs(mouseDy) > 400){
    mouseDy = (mouseDy < 0) ? -1 : 1;
  }
  else 
    mouseDy = mouseDy/400;

  mouseDx *= 100;
  mouseDy *= 100;

  // _playerRigidBody->applyForce(btVector3(mouseDy,0.0,0.0), btVector3(0,0,100));
  // _playerRigidBody->applyForce(btVector3(0.0,mouseDx,0.0), btVector3(0,0,100));

  btTransform tmpTrans;
  _playerRigidBody->getMotionState()->getWorldTransform(tmpTrans);
  btVector3 worldTrans = tmpTrans.getOrigin();
  btMatrix3x3 worldRotation = tmpTrans.getBasis();

  glm::vec3 glmWorldTrans(worldTrans.x(),worldTrans.y(),worldTrans.z());

  glm::mat4 rotationMatrix = swutils::bulletMat3ToGlmMat4(worldRotation);

  glm::vec4 view = glm::vec4(0,0,1,0) *rotationMatrix;

  glm::vec3 lookAt (view.x, view.y, view.z);

  if(_input->getCommandState(_input->FORWARD)){    
    if(_speed < 15)
      _speed += 0.5;
  }

  if(_input->getCommandState(_input->BACKWARD)){
    if(_speed > -15)
      _speed -= 0.5;
  }

  _playerRigidBody->applyCentralForce(swutils::glmVec3ToBulletVec3(glm::normalize(glm::vec3(1,0,0))*_speed*1.0f));

  glm::mat4 translationMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(worldTrans.x(),worldTrans.y(),worldTrans.z()));

  glm::mat4 cameraTrans = glm::translate( glm::mat4(1.0f), playerPos);
  cameraTrans *= rotationMatrix*translationMatrix;
  // cameraTrans *= translationMatrix;
  cameraTrans *= glm::translate(glm::mat4(1.0f), -playerPos);
  _playerTransform = cameraTrans;
}

void Physics::updatePostSync(double dt)
{
  _dynamicsWorld->stepSimulation(dt,100);
}

void Physics::setGravity(float x, float y, float z)
{
  _dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

glm::mat4 Physics::getPlayerTransform()
{
  return _playerTransform;
}