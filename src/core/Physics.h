#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include <btBulletDynamicsCommon.h>
#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/RigidBody.h>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/computeBoundsVisitor>

#include "SwUtils.h"
#include "Input.h"

//! The games physics engine.
class Physics
{

  //friend void tickCallback(btDynamicsWorld * world, btScalar timestep);
  public:

    //! Constructor that initializes all necessary parts of Bullet.
    /*!
        \param input Pointer to the input object used by the main loop to handle player interaction.
    */
    Physics(Input* input);

    //! Basic destructor that removes dynamically allocated resources.
    ~Physics();

    //! Set the gravity for the physics world.
    /*!
        \param x X component of the gravity vector.
        \param y Y component of the gravity vector.
        \param z Z component of the gravity vector.
    */
    void setGravity(float x, float y, float z);

    //! Update that occurs before sgct sync.
    /*!
        Handles player interaction. 
        \param mousePos The current mouse position given by sgct.
        \param resolution The current viewport resolution given by sgct.
        \param playerPos The user position given by the sgct config.
        \param dt The time difference between two updates given by sgct.
    */
    void updatePreSync(int* mousePos, int* resolution, glm::vec3 playerPos, double dt);

    //! Update that occurs after sgct sync.
    /*!
        Currently not used.
    */
    void updatePostSync(double dt);

    //! Used to get the transform that represents the players position in the physics world. 
    /*!
        \return Transform that represents the players position in the physics world.  
    */
    glm::mat4 getPlayerTransform();

    // static btRigidBody* getPlayer() {
    //     return _playerRigidBody;};
    

    //! Test function for collision with ground plane.
    void setUpPlaneCollision();

    //! Adds a collision shape to the physics scene.
    void addCollisionShape(btCollisionShape* shape, glm::mat4 transform);

    //! Get transform for rigid body i.
    glm::mat4 getRigidBodyTransform(int i);

    //! Get transform for rigid body i as osg::Matrix.
    osg::Matrix getRigidBodyTransformAsOsg(int i);

    //! Get number of rigid bodies.
    int getNumberOfRigidBodies();
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
    
    std::vector<btRigidBody*> _rigidBodies;
    std::vector<btCollisionShape*> _collisionShapes;
    std::vector<osgbDynamics::MotionState*> _motionStates;

    // Test member for plane collision
    btCollisionShape* _planeShape;
    btDefaultMotionState* _staticMotionState;
    btRigidBody* _planeRigidBody;

};

#endif