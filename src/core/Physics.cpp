#include "Physics.h"

// void tickCallback(btDynamicsWorld * world, btScalar timestep)
// {
//   // btVector3 velocity = _playerRigidBody->getLinearVelocity();
//   // btScalar speed = velocity.length();
//   // if(speed > 15)
//   // {
//   //   velocity *= 15/speed;
//   //   _playerRigidBody->setLinearVelocity(velocity);
//   // }
// }

Physics::Physics(Input* input)
{
  _input = input;
  _broadphase = new btDbvtBroadphase();
  _collisionConfiguration = new btDefaultCollisionConfiguration();
  _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
  _solver = new btSequentialImpulseConstraintSolver();
  _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
  _dynamicsWorld->setGravity(btVector3(0,0,0));

  // Create player.
  osg::ref_ptr<osg::Node> playerNode;
  playerNode = osgDB::readNodeFile("assets/models/spaceship.3ds");
  if (playerNode.valid())
  {
    std::cout << "Player model found." << std::endl;
    osg::ComputeBoundsVisitor cbv;
    osg::BoundingBox &bb(cbv.getBoundingBox());
    playerNode->accept(cbv);

    _playerShape = osgbCollision::btConvexHullCollisionShapeFromOSG(playerNode);
  }

  // _playerShape = new btBoxShape(btVector3(1.0f,1.0f,1.0f));
  _playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,-5)));
  _playerRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(1, _playerMotionState, _playerShape, btVector3(1,1,1)));

  // setUpPlaneCollision();
  // _dynamicsWorld->addRigidBody(_planeRigidBody);
  _dynamicsWorld->addRigidBody(_playerRigidBody);

  // void (Physics::*callbackPtr) (btDynamicsWorld*, btScalar);
  // callbackPtr = &Physics::tickCallback;
  // _dynamicsWorld->setInternalTickCallback(tickCallback);

}

Physics::~Physics()
{
  delete _dynamicsWorld;
  delete _solver;
  delete _collisionConfiguration;
  delete _dispatcher;
  delete _broadphase;
}

void Physics::updatePreSync(int* mousePos, int* resolution, glm::vec3 playerPos, double dt)
{
  double mouseDx = mousePos[0] - resolution[0]/2.0f;
  double mouseDy = mousePos[1] - resolution[1]/2.0f;

  if(abs(mouseDx) < 100){      
      mouseDx = 0;      
  } 
  else if(abs(mouseDx) > 400){
    mouseDx = (mouseDx < 0) ? 1 : -1;
  }
  else
    mouseDx = mouseDx/400*-1;

  if(abs(mouseDy) < 100)
    mouseDy = 0;
  else if(abs(mouseDy) > 400){
    mouseDy = (mouseDy < 0) ? -1 : 1;
  }
  else 
    mouseDy = mouseDy/400*-1;

  // mouseDx *= 100;
   mouseDy *= 0.1;

  // std::cout << mouseDx << " " << mouseDy << std::endl;
  if (mouseDy == 0 && _playerRigidBody->getAngularVelocity().x() > 0)
  //  _playerRigidBody->applyTorque(btVector3(0,-0.1f,0));
  if (mouseDy == 0 && _playerRigidBody->getAngularVelocity().x() < 0)
  //  _playerRigidBody->applyTorque(btVector3(0,0.1f,0));

  if (mouseDx == 0 && _playerRigidBody->getAngularVelocity().y() > 0){
    // std::cout << "greater than" << std::endl;
    _playerRigidBody->applyTorque(btVector3(0,-0.5f,0));
  }
  if (mouseDx == 0 && _playerRigidBody->getAngularVelocity().y() < 0){
    // std::cout << "less than" << std::endl;
    _playerRigidBody->applyTorque(btVector3(0,0.5f,0));
  }
  if (mouseDx !=0 || mouseDy != 0)
    _playerRigidBody->applyTorque(btVector3(mouseDy,mouseDx,0));
  // std::cout << "Tor: " << _playerRigidBody->getTotalTorque().x() << ' ' << _playerRigidBody->getTotalTorque().y() << std::endl << std::endl;
  // std::cout << "Angular: " << _playerRigidBody->getAngularVelocity().x() << ' ' << _playerRigidBody->getAngularVelocity().y() << std::endl << std::endl;
  // std::cout << "Linear: " << _playerRigidBody->getLinearVelocity().x() << ' ' << _playerRigidBody->getLinearVelocity().y() << std::endl << std::endl;


  btTransform tmpTrans;
  _playerMotionState->getWorldTransform(tmpTrans);
  btVector3 worldTrans = tmpTrans.getOrigin();
  btMatrix3x3 worldRotation = tmpTrans.getBasis();

  glm::vec3 glmWorldTrans(worldTrans.x(),worldTrans.y(),worldTrans.z());

  glm::mat4 rotationMatrix = swutils::bulletMat3ToGlmMat4(worldRotation);

  glm::vec4 view = glm::vec4(0,0,1,0) * rotationMatrix;

  glm::vec3 lookAt (view.x, view.y, view.z);

  if(_input->getCommandState(_input->FORWARD)){    
    if(_speed < 15)
      _speed += 0.5;
  }

  if(_input->getCommandState(_input->BACKWARD)){
    if(_speed > -15)
      _speed -= 0.5;
  }

  if(_input->getCommandState(_input->RIGHT)){
    if(_angle < 15)
      _angle += 0.5;
  }

  if(_input->getCommandState(_input->LEFT)){
    if(_angle > -15)
      _angle -= 0.5;
  }
  // _playerRigidBody->setAngularVelocity(btVector3(mouseDy,mouseDx, _angle));

  _playerRigidBody->applyCentralForce(swutils::glmVec3ToBulletVec3(glm::normalize(lookAt)*_speed));

  glm::mat4 translationMatrix = glm::translate( glm::mat4(1.0f), glm::vec3(worldTrans.x(),worldTrans.y(),worldTrans.z()));

  glm::mat4 cameraTrans = glm::translate( glm::mat4(1.0f), playerPos);
  cameraTrans *= rotationMatrix*translationMatrix;
  // cameraTrans *= translationMatrix;
  cameraTrans *= glm::translate(glm::mat4(1.0f), -playerPos);
  _playerTransform = cameraTrans;

  _dynamicsWorld->stepSimulation(dt,100);

  btVector3 velocity = _playerRigidBody->getLinearVelocity();
  btScalar speed = velocity.length();
  if(speed > 15)
  {
    velocity *= 15/speed;
    _playerRigidBody->setLinearVelocity(velocity);
  }
}

void Physics::updatePostSync(double dt)
{
  
}

void Physics::setGravity(float x, float y, float z)
{
  _dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

glm::mat4 Physics::getPlayerTransform()
{
  return _playerTransform;
}

void Physics::setUpPlaneCollision()
{
    _planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    _staticMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)));
    _planeRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(100,                  // mass
        _staticMotionState,        // initial position
        _planeShape,          // collision shape of body
        btVector3(0,0,0)));
}

void Physics::addCollisionShape(btCollisionShape* shape, glm::mat4 transform)
{
  // _motionStates.push_back(new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0))));

  _motionStates.push_back(new osgbDynamics::MotionState());

  _motionStates.back()->setParentTransform(osg::Matrix(glm::value_ptr(transform)));

  _collisionShapes.push_back(shape);

  _rigidBodies.push_back(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(1, _motionStates.back(), _collisionShapes.back(), btVector3(1,1,1))));

  _dynamicsWorld->addRigidBody(_rigidBodies.back());
}

glm::mat4 Physics::getRigidBodyTransform(int i)
{
  btTransform transform;
  _motionStates[i]->getWorldTransform(transform);

  // return swutils::bulletTransToGlmMat4(transform);

  btVector3 transVec = transform.getOrigin();
  btMatrix3x3 rotMat = transform.getBasis();

  glm::mat4 rotationMatrix = swutils::bulletMat3ToGlmMat4(rotMat);
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transVec.x(), transVec.y(), transVec.z()));

  return rotationMatrix * translationMatrix;

}

osg::Matrix Physics::getRigidBodyTransformAsOsg(int i)
{
  return _motionStates[i]->computeOsgWorldToBulletWorld();
}

int Physics::getNumberOfRigidBodies()
{
  return _rigidBodies.size();
}