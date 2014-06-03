#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <list>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/RigidBody.h>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/computeBoundsVisitor>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/SwUtils.h"

/**
 * @brief Base class representing all objects in the game.
 */
class GameObject
{
  public:
    /**
     * Flags to determine what kind of object to represent.
     */
    enum flags
    {
      PLAYER = 1,
      BULLET = 2
    };

    /**
     * @brief GameObject constructor.
     * @details Initializes the collision object and osg::Node of the GameObject.
     * 
     * @param collisionShape The objects collision shape.
     * @param node The objects osg::Node.
     * @param startTransform Transform that decides the start position and orientation of the object.
     * @param  mass The mass of the object.
     */
    GameObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, int id, float mass = 1.0f);

    ~GameObject();

    /**
     * @brief Get transform representing translation and rotation.
     */
    glm::mat4 getBulletTransform();
    /**
     * @brief Get transform representing translation.
     */
    glm::mat4 getTranslationMatrix();
    /**
     * @brief Get transform representing rotation.
     */
    glm::mat4 getRotationMatrix();

    /**
     * @brief Set the transform for this object.
     * @details This transform is used by osg to update position and orientation.
     */
    void setOsgTransform(glm::mat4 transform);

    /**
     * @brief Get the Bullet collision shape.
     */
    btCollisionShape* getCollisionShape() {
      return _collisionShape;
    }

    /**
     * @brief Get the Bullet motion state.
     */
    osgbDynamics::MotionState* getMotionState() {
      return _motionState;
    }

    /**
     * @brief Get the Bullet RigidBody.
     */
    virtual btRigidBody* getRigidBody() {
      return _rigidBody;
    }

    /**
     * @brief Get the osg::Node.
     */
    osg::Node* getNode() {
      return _node.get();
    }

    osg::MatrixTransform* getOsgMatrix();

    /**
     * @brief Get the identifier of this objects.
     * @return What kind of object this is.
     */
    int getIdentifier() {
      return _identifier;
    }

    /**
     * @brief Flags if the object should be destroyed.
     * @details Same as getDestructionFlag()
     */
    bool shouldBeDestroyed() {
      return _shouldBeDestroyed;
    }

    /**
     * @brief Flags if the object should be destroyed.
     * @details Sama as shouldBeDestroyed.
     */
    bool getDestructionFlag() {
      return _shouldBeDestroyed;
    }

    /**
     * @brief Flag the object to be destroyed.
     * @details Used be physics and renderer to find out if an object needs to be removed.
     */
    void setDestructionFlag(bool flag) {
      _shouldBeDestroyed = flag;
    }

    glm::mat4 getStartTransform() {
        return _startTransform;
    }

    void update();

    static void updateAllObjects();

    static int getNumberOfObjects() {
        return _objects.size();
    }

  protected:
    int _identifier = 0;
    bool _shouldBeDestroyed = false;
    btCollisionShape* _collisionShape;
    osgbDynamics::MotionState* _motionState;
    btRigidBody* _rigidBody;
  
    osg::ref_ptr<osg::MatrixTransform> _transform; 
    osg::ref_ptr<osg::Node> _node;

    float _mass;

    static std::list<GameObject*> _objects;

    glm::mat4 _startTransform;

};
#endif