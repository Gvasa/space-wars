#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "GameObject.h"

/**
 * @brief GameObject representing a bullet fired by a player.
 */
class BulletObject : public GameObject
{
  public:
    BulletObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, glm::vec3 velocity, float mass = 1.0f);
    ~BulletObject();
    
};

#endif