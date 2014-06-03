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


Physics::Physics(Input* input, std::list<BulletObject*>* bulletList)
: MAX_SPEED(15.0f, 15.0f), MIN_SPEED(-15.0f, -15.0f), _speed(0.0f, 0.0f), _bulletList(bulletList)
{
  _input = input;
  _broadphase = new btDbvtBroadphase();
  _collisionConfiguration = new btDefaultCollisionConfiguration();
  _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
  _solver = new btSequentialImpulseConstraintSolver();
  _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
  _dynamicsWorld->setGravity(btVector3(0,0,0));

  // Create player.
  // osg::ref_ptr<osg::Node> playerNode;
  // playerNode = osgDB::readNodeFile("assets/models/fighter.obj");
  // if (playerNode.valid())
  // {
  //   std::cout << "Player model found." << std::endl;
  //   osg::ComputeBoundsVisitor cbv;
  //   osg::BoundingBox &bb(cbv.getBoundingBox());
  //   playerNode->accept(cbv);

  //   _playerShape = osgbCollision::btConvexHullCollisionShapeFromOSG(playerNode);
  // }

  // _playerShape = new btBoxShape(btVector3(1.0f,1.0f,1.0f));
  // _playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,-5)));
  // _playerRigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(1, _playerMotionState, _playerShape, btVector3(1,1,1)));
  // _playerRigidBody->setActivationState(DISABLE_DEACTIVATION);
  // _playerRigidBody->setCollisionFlags(_playerRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  // setUpPlaneCollision();
  // _dynamicsWorld->addRigidBody(_planeRigidBody);<
  // _dynamicsWorld->addRigidBody(_playerRigidBody);

  _dynamicsWorld->setInternalTickCallback([](btDynamicsWorld* world, btScalar timeStep){
      int numOfManifolds = world->getDispatcher()->getNumManifolds();
      for (int i = 0; i < numOfManifolds; i++)
      {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

      //   GameObject* ptrA = static_cast<GameObject*>(obA->getUserPointer());
      //   GameObject* ptrB = static_cast<GameObject*>(obB->getUserPointer());
      // // std::cout << "HIT" << std::endl;
      //   if (ptrA && ptrA->getIdentifier() == GameObject::BULLET)
      //   {
      //     // std::cout << "hejA!" << std::endl;
      //     world->removeRigidBody(ptrA->getRigidBody());
      //     ptrA->setDestructionFlag(true);
      //   }else if (ptrB && ptrB->getIdentifier() == GameObject::BULLET)
      //   {
      //     // std::cout << "hejB!" << std::endl;
      //       world->removeRigidBody(ptrB->getRigidBody());
      //       ptrB->setDestructionFlag(true);
      //   }



        // std::cout << "Colision!" << std::endl;


        // int numContacts = contactManifold->getNumContacts();
        // for (int j=0;j<numContacts;j++)
        // {
        //   btManifoldPoint& pt = contactManifold->getContactPoint(j);
        //   if (pt.getDistance()<0.f)
        //   {
        //     const btVector3& ptA = pt.getPositionWorldOnA();
        //     const btVector3& ptB = pt.getPositionWorldOnB();
        //     const btVector3& normalOnB = pt.m_normalWorldOnB;
        //     // world->removeRigidBody(obB);
        //   } 
        // }

      }
  });

  // void (Physics::*callbackPtr) (btDynamicsWorld*, btScalar);
  // callbackPtr = &Physics::tickCallback;
  // GameObject* player = _player;
  // _dynamicsWorld->setInternalTickCallback([&](btDynamicsWorld *world, btScalar timeStep){
  //   std::cout << "hej" << std::endl;
  //     float mMaxSpeed = 15.0f;
  //     // btVector3 velocity = player->getRigidBody()->getLinearVelocity();
  //     // btScalar speed = velocity.length();
  //     // if(speed > mMaxSpeed) {
  //     //     velocity *= mMaxSpeed/speed;
  //     //     _player->getRigidBody()->setLinearVelocity(velocity);
  //     // }

  // });

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

}

void Physics::updatePostSync(double dt)
{

  _translationMatrix = _player->getTranslationMatrix();
  _rotationMatrix = _player->getRotationMatrix();


  glm::vec3 playerPos = Info::getPlayerPosition();

  glm::mat4 cameraTrans = glm::translate( glm::mat4(1.0f), playerPos);
  cameraTrans *= _rotationMatrix*_translationMatrix;
  cameraTrans *= glm::translate(glm::mat4(1.0f), -playerPos);

  _playerTransform = cameraTrans;

  if (_input->getCommandState(_input->FORWARD))
  {
    if (_speed.x < MAX_SPEED.x)
      _speed.x += 0.5;
  }
  if (_input->getCommandState(_input->BACKWARD))
  {
    if (_speed.x > MIN_SPEED.x)
      _speed.x -= 0.5;
  }

  if (_input->getCommandState(_input->LEFT))
  {
    if (_speed.y > MIN_SPEED.y)
      _speed.y -= 0.5;
  }
  if (_input->getCommandState(_input->RIGHT))
  {
    if (_speed.y < MAX_SPEED.y)
      _speed.y += 0.5;
  }
  if (_input->getCommandState(_input->TILT_LEFT))
  {
    if (_tilt > MIN_TILT)
      _tilt -= 0.1;
  }
  if (_input->getCommandState(_input->TILT_RIGHT))
  {
    if (_tilt < MAX_TILT)
      _tilt += 0.1;
  }

  btRigidBody* playerRigidBody = _player->getRigidBody();

  btVector3 btLinearVelocity = playerRigidBody->getLinearVelocity();
  // glm::vec2 uLinear(_speed.x - btLinearVelocity.z(), _speed.y - btLinearVelocity.x());
  glm::vec4 playerDirection = glm::vec4(0, 0, 1, 0) * _rotationMatrix;
  glm::vec4 playerUp = glm::vec4(0, 1, 0, 0) * _rotationMatrix;
  glm::vec4 playerRight = glm::vec4(-1, 0, 0, 0) * _rotationMatrix;

  glm::vec4 playerSpeed =  glm::normalize(glm::vec4(0, 0, 1, 0) * _rotationMatrix);
  playerSpeed *= _speed.x;

  glm::vec3 uLinear = glm::vec3(playerSpeed.x - btLinearVelocity.x(), playerSpeed.y - btLinearVelocity.y(), playerSpeed.z - btLinearVelocity.z());
  // glm::vec4(uLinear.y, 0, uLinear.x, 0);
  uLinear *= 10.0f;

  // glm::vec4 centralForce = glm::vec4(uLinear.y, 0, uLinear.x, 0) * swutils::bulletMat3ToGlmMat4(playerRotation) ;
  playerRigidBody->applyCentralForce(btVector3(uLinear.x, uLinear.y, uLinear.z));
  

  float halfWidth = Info::getXresolution()/2.0f;
  float halfHeight = Info::getYresolution()/2.0f;

  float dx = -(_input->getMousePositionX()/halfWidth - 1)/4.0f;
  float dy = -(_input->getMousePositionY()/halfHeight - 1)/4.0f;
  // std::cout << abs(dx) << " " << abs(dy) << std::endl;
  dx = (dx < 0.1 && dx > -0.1) ? 0 : dx;
  dy = (dy < 0.1 && dy > -0.1) ? 0 : dy;

  // std::cout << dx << " " << dy << std::endl;
  btVector3 btAngularVelocity = playerRigidBody->getAngularVelocity();
  // playerRigidBody->setAngularVelocity(btVector3(btAngularVelocity.x(), btAngularVelocity.y(), 0));
  float gain = 10.0f;
  glm::vec3 uAngular(gain*(dx - btAngularVelocity.y()), gain*(dy - btAngularVelocity.x()), 0);
  float uTilt = gain * (_tilt - btAngularVelocity.z());

  glm::vec4 angularUp = glm::vec4(0,0,uAngular.y,0) * _rotationMatrix;
  glm::vec4 angularRight = glm::vec4(0,0,uAngular.x,0) * _rotationMatrix;

  double limit = 5.0;
  double reset = 0;
  if (angularUp.x > limit)
    angularUp.x = reset;
  if (angularUp.x < -limit)
    angularUp.x = -reset;

  if (angularUp.y > limit)
    angularUp.y = reset;
  if (angularUp.y < -limit)
    angularUp.y = -reset;

  if (angularUp.z > limit)
    angularUp.z = reset;
  if (angularUp.z < -limit)
    angularUp.z = -reset;

  playerRigidBody->applyForce(btVector3(angularUp.x, angularUp.y, angularUp.z), btVector3(playerUp.x, playerUp.y, playerUp.z));
  
  
  if (angularRight.x > limit)
    angularRight.x = reset;
  if (angularRight.x < -limit)
    angularRight.x = -reset;

  if (angularRight.y > limit)
    angularRight.y = reset;
  if (angularRight.y < -limit)
    angularRight.y = -reset;

  if (angularRight.z > limit)
    angularRight.z = reset;
  if (angularRight.z < -limit)
    angularRight.z = -reset;

  if (angularRight.x > limit || angularRight.y > limit || angularRight.z > limit || angularUp.x > limit || angularUp.y > limit || angularUp.z > limit || 
    angularRight.x < -limit || angularRight.y < -limit || angularRight.z < -limit || angularUp.x < -limit || angularUp.y < -limit || angularUp.z < -limit)
  {
    _dynamicsWorld->removeRigidBody(playerRigidBody);
    playerRigidBody->clearForces();
    playerRigidBody->setAngularVelocity(btVector3(0,0,0));
    playerRigidBody->setLinearVelocity(btVector3(0,0,0));
    _speed = glm::vec2(0.0f,0.0f);
    playerRigidBody->setWorldTransform(swutils::glmMat4ToBullletTranform(_player->getStartTransform()));
    _dynamicsWorld->addRigidBody(playerRigidBody);
  }

  playerRigidBody->applyForce(btVector3(angularRight.x, angularRight.y, angularRight.z), btVector3(playerRight.x, playerRight.y, playerRight.z));

  Info::setPlayerLinearVelocity(glm::vec3(btLinearVelocity.x(), btLinearVelocity.y(), btLinearVelocity.z()));
  Info::setPlayerAngularVelocity(glm::vec3(btAngularVelocity.x(), btAngularVelocity.y(), btAngularVelocity.z()));
  // Info::setPlayerAngularVelocity(glm::vec3(playerUp.x, playerUp.y, playerUp.z));

  for (auto it = _bulletList->begin(); it != _bulletList->end(); it++){
    (*it)->getRigidBody()->applyCentralForce(btVector3(playerDirection.x,playerDirection.y,playerDirection.z));
  }

  _dynamicsWorld->stepSimulation(dt);

  if (_input->getCommandState(_input->RESET))
  {
    _dynamicsWorld->removeRigidBody(playerRigidBody);
    playerRigidBody->clearForces();
    playerRigidBody->setAngularVelocity(btVector3(0,0,0));
    playerRigidBody->setLinearVelocity(btVector3(0,0,0));
    _speed = glm::vec2(0.0f,0.0f);
    playerRigidBody->setWorldTransform(swutils::glmMat4ToBullletTranform(_player->getStartTransform()));
    _dynamicsWorld->addRigidBody(playerRigidBody);
  }
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
  // _rigidBodies.back()->setCollisionFlags(_rigidBodies.back()->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  _dynamicsWorld->addRigidBody(_rigidBodies.back());
  // _dynamicsWorld->contactPairTest(_rigidBodies.back(), _playerRigidBody, _collisionCallback);
}

void Physics::addObject(GameObject* obj)
{
  _dynamicsWorld->addRigidBody(obj->getRigidBody());
  // _motionStates.push_back(obj->getMotionState());
  // _collisionShapes.push_back(obj->getCollisionShape());
  // _rigidBodies.push_back(obj->getRigidBody());
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

void Physics::registerPlayerObject(GameObject* player)
{
  _player = player;
  _dynamicsWorld->addRigidBody(_player->getRigidBody());
}