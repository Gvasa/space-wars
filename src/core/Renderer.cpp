#include "Renderer.h"

Renderer::Renderer()
{
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
}

Renderer::~Renderer()
{
  delete _viewer;
}

void Renderer::addObject()
{

}