#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "sgct.h"
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

class RenderObject
{
  public:
   RenderObject();
   ~RenderObject();

   void setMatrixTransform(osg::Matrixd matrix);

   osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
   osg::ref_ptr<osg::Node> getModelNode();

  private:
    osg::ref_ptr<osg::MatrixTransform> _modelTransform;
    osg::ref_ptr<osg::Node> _model;
  
};

#endif