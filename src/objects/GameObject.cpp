#include "GameObject.h"

GameObject::GameObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, float mass)
: _collisionShape(collisionShape), _node(node), _mass(mass)
{
  _motionState = new osgbDynamics::MotionState();
  _motionState->setParentTransform(osg::Matrix(glm::value_ptr(startTransform)));
  _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(1, _motionState, _collisionShape, btVector3(1,1,1)));

  _rigidBody->setActivationState(DISABLE_DEACTIVATION);

  _transform = new osg::MatrixTransform();
  _transform->addChild(node);
}

GameObject::~GameObject()
{

}

glm::mat4 GameObject::getBulletTransform()
{
  btTransform transform;
  _motionState->getWorldTransform(transform);

  btVector3 transVec = transform.getOrigin();
  btMatrix3x3 rotMat = transform.getBasis();

  glm::mat4 rotationMatrix = swutils::bulletMat3ToGlmMat4(rotMat);
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transVec.x(), transVec.y(), transVec.z()));

  return rotationMatrix * translationMatrix;
}

glm::mat4 GameObject::getTranslationMatrix()
{
  btTransform transform;
  _motionState->getWorldTransform(transform);

  btVector3 transVec = transform.getOrigin();

  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transVec.x(), transVec.y(), transVec.z()));

  return translationMatrix;
}

glm::mat4 GameObject::getRotationMatrix()
{
  btTransform transform;
  _motionState->getWorldTransform(transform);

  btMatrix3x3 rotMat = transform.getBasis();

  glm::mat4 rotationMatrix = swutils::bulletMat3ToGlmMat4(rotMat);

  return rotationMatrix;
}

void GameObject::setOsgTransform(glm::mat4 transform)
{
  _transform->setMatrix(osg::Matrix(glm::value_ptr(transform)));
}