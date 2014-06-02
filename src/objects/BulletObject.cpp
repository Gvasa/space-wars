#include "BulletObject.h"

BulletObject::BulletObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, glm::vec3 velocity, float mass)
: GameObject(collisionShape, node, startTransform, BULLET, mass)
{
  // _rigidBody->setLinearVelocity(btVector3(velocity.x,velocity.x,velocity.z));
}
