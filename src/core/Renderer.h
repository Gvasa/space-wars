#ifndef RENDERER_H
#define RENDERER_H

#include <list>

#include "sgct.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

class Renderer
{
  public:
    Renderer();
    ~Renderer();
    
    void addObject();
  private:
    osgViewer::Viewer* _viewer;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::FrameStamp> _frameStamp;
};

#endif