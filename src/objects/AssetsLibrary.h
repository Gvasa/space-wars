#ifndef ASSETSLIBRARY_H
#define ASSETSLIBRARY_H

#include <vector>
#include <string>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/RigidBody.h>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/computeBoundsVisitor>

#define SHOW_COLLISION_SHAPES 0

/**
 * @brief Static class the stores all 3d models and collision shapes.
 */
class AssetsLibrary 
{
  public:
    enum id
    {
      FIGHTER = 0,
      BULLET = 1,
      ASTEROID = 2,
      MOTHERSHIP = 3
    };

    AssetsLibrary();
    ~AssetsLibrary();

    static void init();

    /**
     * @brief Add a model and create a collisionshape.
     */
    static void addModel(std::string fileName);
    /**
     * @brief Get the osg::Node* representing object with id nodeId.
     */
    static osg::Node* getNode(int nodeId);
    /**
     * @brief Get the collision shape representing object with id shapeId.
     */
    static btCollisionShape* getCollisionShape(int shapeId);
  
  private:
    static std::vector<osg::Node*> _nodes;
    static std::vector<btCollisionShape*> _collisionShapes;
};

#endif