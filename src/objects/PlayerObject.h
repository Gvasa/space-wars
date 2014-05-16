#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "GameObject.h"

/**
 * @brief GameObject representing a player.
 * @details Contains HP and other player related data in addition to those found in GameObejct.
 */
class PlayerObject : public GameObject
{
  public:
    PlayerObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, float mass = 1.0f);
    ~PlayerObject();
  
};

#endif