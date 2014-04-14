#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include "SwUtils.h"
#include "Input.h"

class Physics
{
  public:
    Physics(Input* input);
    ~Physics();

    void setGravity(float x, float y, float z);

    void updatePreSync(int* mousePos, int* resolution, glm::vec3 playerPos);
    void updatePostSync(double dt);

    glm::mat4 getPlayerTransform();

  private:
    btBroadphaseInterface* _broadphase;
    btDefaultCollisionConfiguration* _collisionConfiguration;
    btCollisionDispatcher* _dispatcher;
    btSequentialImpulseConstraintSolver* _solver;
    btDiscreteDynamicsWorld* _dynamicsWorld;

    btCollisionShape* _playerShape;
    btDefaultMotionState* _playerMotionState;
    btRigidBody* _playerRigidBody;

    float _speed = 10.0f;
    glm::mat4 _playerTransform;
    Input* _input;
};

#endif