#include "Physics.h"

Physics::Physics()
{
  _broadphase = new btDbvtBroadphase();
  _collisionConfiguration = new btDefaultCollisionConfiguration();
  _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
  _solver = new btSequentialImpulseConstraintSolver();
  _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_broadphase,_solver,_collisionConfiguration);
}

Physics::~Physics()
{
  delete _broadphase;
  delete _collisionConfiguration;
  delete _dispatcher;
  delete _solver;
  delete _dynamicsWorld;
}

void Physics::setGravity(float x, float y, float z)
{
  _dynamicsWorld->setGravity(btVector3(x, y, z));
}