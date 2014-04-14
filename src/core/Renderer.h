#ifndef RENDERER_H
#define RENDERER_H

#include <list>

#include "sgct.h"
#include <glm/glm.hpp>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

class Renderer
{
  public:
    Renderer();
    ~Renderer();
    
    void render();
    void update(double currentTime,  unsigned int frameNumber);

    void setProjectionMatrix(osg::Matrix mat);
    void setPixelCoords(int vp1, int vp2, int vp3, int vp4);
    void addObject();
  private:
    osgViewer::Viewer* _viewer;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::MatrixTransform> _sceneTransform;
    osg::ref_ptr<osg::FrameStamp> _frameStamp;

    osg::Matrix _projectionMatrix;
    int* _pixelCoords;
};

#endif