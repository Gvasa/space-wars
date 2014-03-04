#include "RenderObject.h"

RenderObject::RenderObject()
{
  _modelTransform = new osg::MatrixTransform();
  _modelTransform->preMult(osg::Matrix::rotate(glm::radians(-90.0f),
                                            1.0f, 0.0f, 0.0f));

  _model = osgDB::readNodeFile("assets/models/spaceship.3ds");
 
  if (_model.valid())
  {
    sgct::MessageHandler::instance()->print("Model loaded successfully!\n");

    osg::ComputeBoundsVisitor cbv;
    osg::BoundingBox &bb(cbv.getBoundingBox());
    _model->accept( cbv );
  }else
    sgct::MessageHandler::instance()->print("Failed to read model!\n");
    

}

RenderObject::~RenderObject()
{

}

void RenderObject::setMatrixTransform(osg::Matrixd matrix)
{
  _modelTransform->setMatrix(matrix);
}

osg::ref_ptr<osg::MatrixTransform> RenderObject::getMatrixTransform()
{
  return _modelTransform;
}

osg::ref_ptr<osg::Node> RenderObject::getModelNode()
{
  return _model;
}