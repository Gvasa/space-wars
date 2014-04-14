#include "Renderer.h"

Renderer::Renderer()
{
  _pixelCoords = new int[4];
  _root = new osg::Group();
  osg::Referenced::setThreadSafeReferenceCounting(true);
  _frameStamp = new osg::FrameStamp();

  _viewer = new osgViewer::Viewer();
  _viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  osg::ref_ptr<::osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
      new osgViewer::GraphicsWindowEmbedded(traits.get());

  _viewer->getCamera()->setGraphicsContext(graphicsWindow.get());

  _viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
  _viewer->getCamera()->setClearColor(osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f));

  GLbitfield clearMask = _viewer->getCamera()->getClearMask();
  _viewer->getCamera()->setClearMask(clearMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));

  _viewer->setSceneData(_root.get());

  _sceneTransform = new osg::MatrixTransform();
  _root->addChild(_sceneTransform.get());

}

Renderer::~Renderer()
{
  delete _viewer;
  delete _pixelCoords;
}

void Renderer::render()
{
  _viewer->getCamera()->setViewport(_pixelCoords[0], _pixelCoords[1], _pixelCoords[2], _pixelCoords[3]);
  _viewer->getCamera()->setProjectionMatrix(_projectionMatrix);

  _viewer->renderingTraversals();

}

void Renderer::update(double currentTime, unsigned int frameNumber)
{
  _frameStamp->setFrameNumber(frameNumber);
  _frameStamp->setReferenceTime(currentTime);
  _frameStamp->setSimulationTime(currentTime);
  _viewer->setFrameStamp(_frameStamp.get());
  _viewer->advance(currentTime);

  if (!_viewer->done())
  {
    _viewer->eventTraversal();
    _viewer->updateTraversal();
  }
}

void Renderer::setProjectionMatrix(osg::Matrix mat)
{
  _projectionMatrix = mat;
}

void Renderer::setPixelCoords(int vp1, int vp2, int vp3, int vp4)
{
  _pixelCoords[0] = vp1;
  _pixelCoords[1] = vp2;
  _pixelCoords[2] = vp3;
  _pixelCoords[3] = vp4;
}

void Renderer::addObject()
{

}