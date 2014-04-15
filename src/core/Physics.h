#ifndef PHYSICS_H
#define PHYSICS_H

#include <btBulletDynamicsCommon.h>

#include "SwUtils.h"
#include "Input.h"

class Physics
{

// friend void tickCallback(btDynamicsWorld * world, btScalar timestep);
  public:
    Physics(Input* input);
    ~Physics();

    void setGravity(float x, float y, float z);

    void updatePreSync(int* mousePos, int* resolution, glm::vec3 playerPos);
    void updatePostSync(double dt);

    glm::mat4 getPlayerTransform();

    // static btRigidBody* getPlayer() {
    //     return _playerRigidBody;};
    

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
    float _angle = 0.0f;
    glm::mat4 _playerTransform;
    Input* _input;
    
};

#endif