#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>

#include "sgct.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/TexGen>

class Renderer
{
  public:
    Renderer();
    ~Renderer();
    
    void render();
    void updatePreSync(double currentTime);
    void updatePostSync(double currentTime,  unsigned int frameNumber, glm::mat4 modelMatrix);

    void setProjectionMatrix(glm::mat4 mat);
    void setPixelCoords(int vp1, int vp2, int vp3, int vp4);
    void addObject();
    void setSceneTransform(glm::mat4 transform);

    void tempSetUpLight();

  private:
    osgViewer::Viewer* _viewer;
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<osg::MatrixTransform> _sceneTransform;
    osg::ref_ptr<osg::FrameStamp> _frameStamp;

    osg::Matrix _projectionMatrix;
    int* _pixelCoords;
};

#endif