#include "PlayerObject.h"

PlayerObject::PlayerObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, float mass)
: GameObject(collisionShape, node, startTransform, mass)
{

}