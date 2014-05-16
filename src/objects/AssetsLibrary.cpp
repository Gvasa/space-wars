#include "objects/AssetsLibrary.h"
#include <iostream>
std::vector<osg::Node*> AssetsLibrary::_nodes;
std::vector<btCollisionShape*> AssetsLibrary::_collisionShapes;

AssetsLibrary::AssetsLibrary()
{

}

void AssetsLibrary::init()
{
  addModel("assets/models/fighter_material.obj");
  addModel("assets/models/skott.obj");
  addModel("assets/models/asteroid.obj");
}

void AssetsLibrary::addModel(std::string fileName)
{
  osg::Node* node = osgDB::readNodeFile(fileName.c_str());
  
  _collisionShapes.push_back(osgbCollision::btConvexHullCollisionShapeFromOSG(node));

  #if SHOW_COLLISION_SHAPES == 1
    _nodes.push_back(osgbCollision::osgNodeFromBtCollisionShape(_collisionShapes.back()));
  #else
    _nodes.push_back(node);
  #endif
}

osg::Node* AssetsLibrary::getNode(int nodeId)
{
  return _nodes[nodeId];
}

btCollisionShape* AssetsLibrary::getCollisionShape(int shapeId)
{
  return _collisionShapes[shapeId];
}