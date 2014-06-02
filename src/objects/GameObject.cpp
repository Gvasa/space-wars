#include "GameObject.h"

std::list<GameObject*> GameObject::_objects;

GameObject::GameObject(btCollisionShape* collisionShape, osg::Node* node, glm::mat4 startTransform, int id, float mass)
: _collisionShape(collisionShape), _node(node), _mass(mass), _identifier(id)
{
  _motionState = new osgbDynamics::MotionState();
  _motionState->setParentTransform(osg::Matrix(glm::value_ptr(startTransform)));
  _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, _motionState, _collisionShape, btVector3(1,1,1)));

  _rigidBody->setActivationState(DISABLE_DEACTIVATION);
  _rigidBody->setUserPointer(this);

  _transform = new osg::MatrixTransform();
  _transform->addChild(node);

  GameObject::_objects.push_back(this);
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

osg::MatrixTransform* GameObject::getOsgMatrix()
{
  _transform->setMatrix(osg::Matrix(glm::value_ptr(glm::inverse(getBulletTransform()))));
  return _transform.get();
}

void GameObject::update()
{
  _transform->setMatrix(osg::Matrix(glm::value_ptr(glm::inverse(getBulletTransform()))));
}

void GameObject::updateAllObjects()
{
  // std::cout << "GameObject::updateAllObjects()" << std::endl;
  for (auto it = GameObject::_objects.begin(); it != GameObject::_objects.end(); it++)
  {
    if ((*it)->shouldBeDestroyed()) {
      // delete *it;
      GameObject::_objects.erase(it);
    }
    else
      (*it)->update();
      
  }
}