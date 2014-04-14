#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h> 

#include "sgct.h"

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>

sgct::Engine* _engine;

osgViewer::Viewer* _viewer;
osg::ref_ptr<osg::Group> _root;
osg::ref_ptr<osg::MatrixTransform> _sceneTransform;
osg::ref_ptr<osg::FrameStamp> _frameStamp;

void init();
void preSync();
void postSyncPreDraw();
void draw();
void encode();
void decode();
void cleanUp();
void clientCallback(const char * receivedChars, int size, int clientId);
void keyCallback(int key, int action);
void mouseButtonCallback(int key, int action);

// Other functions.
void addModel(std::string fileName);
double strToDouble(std::string value);

sgct::SharedDouble curr_time(0.0);

std::vector<osg::ref_ptr<osg::Node>> _models;
std::vector<osg::ref_ptr<osg::MatrixTransform>> _modelTransforms;
std::vector<osg::Vec4d> _boundingBoxInfos;
int selectedModel = 0;
bool first = true;
osg::Vec3d translation;
osg::Vec3d rotation;

// Camera control variables.
bool arrowButtons[4];
enum directions { FORWARD = 0, BACKWARD, LEFT, RIGHT };
bool _leftMousePressed = false;

double _mouseDx = 0.0;
double _mouseDy = 0.0;

double _mouseXPos = 0.0;
double _mouseYPos = 0.0;
double _mouseClickXPos = 0.0;
double _mouseClickYPos = 0.0;

float _rotationSpeed = 4.0f;
float _rollSpeed = 50.0f;
float _walkingSpeed = 10.0f;

glm::vec3 view(0.0f, 0.0f, 1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 pos(0.0f, 0.0f, 0.0f);

glm::mat4 result;

int main(int argc, char* argv[])
{
  _engine = new sgct::Engine(argc, argv);

  _engine->setInitOGLFunction(init);
  _engine->setPreSyncFunction(preSync);
  _engine->setPostSyncPreDrawFunction(postSyncPreDraw);
  _engine->setDrawFunction(draw);
  _engine->setCleanUpFunction(cleanUp);
  _engine->setExternalControlCallback(clientCallback);
  _engine->setKeyboardCallbackFunction(keyCallback);
  _engine->setMouseButtonCallbackFunction(mouseButtonCallback);

  for(int i=0; i<4; i++)
    arrowButtons[i] = false;

  if( !_engine->init() )
  {
    delete _engine;
    return EXIT_FAILURE;
  }

  sgct::SharedData::instance()->setEncodeFunction(encode);
  sgct::SharedData::instance()->setDecodeFunction(decode);

  _engine->render();

  delete _engine;

  exit(EXIT_SUCCESS);
}

void init()
{
  // Set up osg as embeded for use in sgct.
  _root = new osg::Group();
  osg::Referenced::setThreadSafeReferenceCounting(true);

  _viewer = new osgViewer::Viewer;
  _frameStamp = new osg::FrameStamp();

  _viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  osg::ref_ptr< ::osg::GraphicsContext::Traits > traits =
      new osg::GraphicsContext::Traits;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
      new osgViewer::GraphicsWindowEmbedded(traits.get());

  _viewer->getCamera()->setGraphicsContext(graphicsWindow.get());

  _viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
  _viewer->getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f) );

  GLbitfield tmpMask = _viewer->getCamera()->getClearMask();
  _viewer->getCamera()->setClearMask(tmpMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));

  _viewer->setSceneData(_root.get());

  // Set up lights.
  osg::Light * light0 = new osg::Light;
  osg::Light * light1 = new osg::Light;
  osg::LightSource* lightSource0 = new osg::LightSource;
  osg::LightSource* lightSource1 = new osg::LightSource;

  light0->setLightNum( 0 );
  light0->setPosition( osg::Vec4( 5.0f, 5.0f, 10.0f, 1.0f ) );
  light0->setAmbient( osg::Vec4( 1.0f, 0.3f, 0.3f, 1.0f ) );
  light0->setDiffuse( osg::Vec4( 0.8f, 0.8f, 0.8f, 1.0f ) );
  light0->setSpecular( osg::Vec4( 0.1f, 0.1f, 0.1f, 1.0f ) );
  light0->setConstantAttenuation( 1.0f );

  lightSource0->setLight( light0 );
  lightSource0->setLocalStateSetModes( osg::StateAttribute::ON );
  lightSource0->setStateSetModes( *(_root->getOrCreateStateSet()), osg::StateAttribute::ON );

  light1->setLightNum( 1 );
  light1->setPosition( osg::Vec4( -5.0f, -2.0f, 10.0f, 1.0f ) );
  light1->setAmbient( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
  light1->setDiffuse( osg::Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
  light1->setSpecular( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
  light1->setConstantAttenuation( 1.0f );

  lightSource1->setLight( light1 );
  lightSource1->setLocalStateSetModes( osg::StateAttribute::ON );
  lightSource1->setStateSetModes( *(_root->getOrCreateStateSet()), osg::StateAttribute::ON );

  _root->addChild( lightSource0 );
  _root->addChild( lightSource1 );

  // Set up scene transform.
  _sceneTransform = new osg::MatrixTransform();
  _root->addChild(_sceneTransform.get());

  // addModel("airplane.ive");

}

void preSync()
{
  if( _engine->isMaster() )
  {
    curr_time.setVal( sgct::Engine::getTime() );

    static float panRot = 0.0f;
    static float vertRot = 0.0f;
    if (_leftMousePressed)
    {
      int width, height;
      width = _engine->getActiveXResolution();
      height = _engine->getActiveYResolution();
      sgct::Engine::getMousePos(_engine->getFocusedWindowIndex(), &_mouseXPos, &_mouseYPos);

      _mouseDx = _mouseXPos - _mouseClickXPos;
      _mouseDy = _mouseYPos - _mouseClickYPos;

      sgct::Engine::setMousePos(_engine->getFocusedWindowIndex(), _mouseClickXPos, _mouseClickYPos);

      
      panRot += (static_cast<float>(_mouseDx) * _rotationSpeed * static_cast<float>(_engine->getDt()));
      
      vertRot += (static_cast<float>(_mouseDy) * _rotationSpeed * static_cast<float>(_engine->getDt()));
    }

    glm::mat4 ViewRotateX = glm::rotate(
      glm::mat4(1.0f),
      panRot,
      glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around the y-axis

    glm::mat4 ViewRotateY = glm::rotate(
      glm::mat4(1.0f),
      vertRot,
      glm::vec3(1.0f, 0.0f, 0.0f)); //rotation around the x-axis

    glm::mat4 ViewMat = ViewRotateY * ViewRotateX;  
    view = glm::inverse(glm::mat3(ViewMat)) * glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 right = glm::cross(view, up);

    if( arrowButtons[FORWARD] )
      pos += (_walkingSpeed * static_cast<float>(_engine->getDt()) * view);
    if( arrowButtons[BACKWARD] )
      pos -= (_walkingSpeed * static_cast<float>(_engine->getDt()) * view);
    if( arrowButtons[LEFT] )
      pos -= (_walkingSpeed * static_cast<float>(_engine->getDt()) * right);
    if( arrowButtons[RIGHT] )
      pos += (_walkingSpeed * static_cast<float>(_engine->getDt()) * right);

    result = glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
    //2. apply transformation
    result *= (ViewMat *  glm::translate( glm::mat4(1.0f), pos ));
    //1. transform user to coordinate system origin
    result *= glm::translate( glm::mat4(1.0f), -sgct::Engine::getUserPtr()->getPos() );


    _sceneTransform->setMatrix(osg::Matrixd(glm::value_ptr(result)));
  }
}

void postSyncPreDraw()
{
  _root->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

  // _sceneTransform->setMatrix(osg::Matrix::rotate( glm::radians(curr_time.getVal() * 8.0), 0.0, 1.0, 0.0));
  //_sceneTransform->setMatrix(osg::Matrix::translate(0.0, -0.1, -2));
  _sceneTransform->postMult( osg::Matrix( glm::value_ptr(_engine->getModelMatrix())));

  if (_modelTransforms.size() > selectedModel)
  {
    double radius = _boundingBoxInfos.at(selectedModel).w();
    glm::vec3 tmpVec(_boundingBoxInfos.at(selectedModel).x(), _boundingBoxInfos.at(selectedModel).y(), _boundingBoxInfos.at(selectedModel).z());
    //_modelTransforms.at(selectedModel)->setMatrix(osg::Matrixd(glm::value_ptr(glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));
    _modelTransforms.at(selectedModel)->setMatrix(osg::Matrixd(glm::value_ptr(glm::translate(glm::mat4(1), -tmpVec))));
    _modelTransforms.at(selectedModel)->postMult(osg::Matrixd(glm::value_ptr(glm::scale(glm::mat4(1), glm::vec3(1.0f/radius, 1.0f/radius, 1.0f/radius )))));
    
    _modelTransforms.at(selectedModel)->postMult(osg::Matrixd(glm::value_ptr(glm::rotate(glm::mat4(1), (float) rotation.x(), glm::vec3(1.0f, 0.0f, 0.0f)))));
    _modelTransforms.at(selectedModel)->postMult(osg::Matrixd(glm::value_ptr(glm::rotate(glm::mat4(1), (float) rotation.y(), glm::vec3(0.0f, 1.0f, 0.0f)))));
    _modelTransforms.at(selectedModel)->postMult(osg::Matrixd(glm::value_ptr(glm::rotate(glm::mat4(1), (float) rotation.z(), glm::vec3(0.0f, 0.0f, 1.0f)))));

    _modelTransforms.at(selectedModel)->postMult(osg::Matrixd(glm::value_ptr(glm::translate(glm::mat4(1), glm::vec3(translation.x(), translation.y(), translation.z())))));

  }


  _frameStamp->setFrameNumber(_engine->getCurrentFrameNumber());
  _frameStamp->setReferenceTime(curr_time.getVal());
  _frameStamp->setSimulationTime(curr_time.getVal());
  _viewer->setFrameStamp(_frameStamp.get());
  _viewer->advance(curr_time.getVal()); //update

  //traverse if there are any tasks to do
  if (!_viewer->done())
  {
    _viewer->eventTraversal();
    //update travelsal needed for pagelod object like terrain data etc.
    _viewer->updateTraversal();
  }

}

void draw()
{
  const int * curr_vp = _engine->getActiveViewportPixelCoords();
  _viewer->getCamera()->setViewport(curr_vp[0], curr_vp[1], curr_vp[2], curr_vp[3]);
  _viewer->getCamera()->setProjectionMatrix( osg::Matrix( glm::value_ptr(_engine->getActiveViewProjectionMatrix() ) ));

  _viewer->renderingTraversals();
}

void encode()
{
  sgct::SharedData::instance()->writeDouble( &curr_time );
}

void decode()
{
  sgct::SharedData::instance()->readDouble( &curr_time );
}

void cleanUp()
{
  delete _viewer;
  _viewer = NULL;
}

void clientCallback(const char * receivedChars, int size, int clientId)
{
  if( _engine->isMaster() )
  { 
    if (strncmp(receivedChars, "model", 5) == 0)
    {
      sgct::MessageHandler::instance()->print("Received model. \n");
      std::string fileName(receivedChars + 6, receivedChars + size);
      addModel(fileName);

    }else if (strncmp(receivedChars, "delete", 6) == 0)
    {
      _modelTransforms.at(selectedModel)->setNodeMask(0x0);
    }else if (strncmp(receivedChars, "positionX", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.x() = strToDouble(tmp)/200;
      
    }else if (strncmp(receivedChars, "positionY", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.y() = strToDouble(tmp)/200;
    }else if (strncmp(receivedChars, "positionZ", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.z() = strToDouble(tmp)/200;
    }else if (strncmp(receivedChars, "rotationX", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.x() = strToDouble(tmp)/20;
      // std::cout << rotation.x() << std::endl;
    }else if (strncmp(receivedChars, "rotationY", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.y() = strToDouble(tmp)/20;
    }else if (strncmp(receivedChars, "rotationZ", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.z() = strToDouble(tmp)/20;
    }else if (strncmp(receivedChars, "selectedModel", 13) == 0)
    {
      std::string tmp(receivedChars + 14, receivedChars + size);
      selectedModel = strToDouble(tmp);
    }
  }
}

void addModel(std::string fileName)
{
  sgct::MessageHandler::instance()->print("Loading model \n");
  _models.push_back(osgDB::readNodeFile(fileName));

  if (_models.back().valid())
  {
    sgct::MessageHandler::instance()->print("Successfully loaded model. \n");

    _modelTransforms.push_back(new osg::MatrixTransform());
    
   // _modelTransforms.back()->setMatrix(osg::Matrixd(glm::value_ptr(glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))));

    _modelTransforms.back()->addChild(_models.back().get());

    osg::ComputeBoundsVisitor cbv;
    osg::BoundingBox &bb(cbv.getBoundingBox());
    
    _modelTransforms.back()->accept( cbv );

    osg::Vec3f tmpVec;
    tmpVec = bb.center();

    _boundingBoxInfos.push_back(osg::Vec4d(tmpVec, bb.radius()));

    
    _modelTransforms.back()->setMatrix(osg::Matrixd(glm::value_ptr(glm::translate(glm::mat4(1), glm::vec3(-tmpVec.x(), -tmpVec.y(), -tmpVec.z())))));
    _modelTransforms.back()->postMult(osg::Matrixd(glm::value_ptr(glm::scale(glm::mat4(1), glm::vec3(1.0f/bb.radius(), 1.0f/bb.radius(), 1.0f/bb.radius())))));
   
    if (_modelTransforms.size() == 1)
    {
      _sceneTransform->addChild(_modelTransforms.back());
    }else 
      _modelTransforms.front()->addChild(_modelTransforms.back());

    _models.back()->getOrCreateStateSet()->setMode( GL_CULL_FACE,
      osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    _models.back()->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

  }else
  {
    sgct::MessageHandler::instance()->print("Failed to read model. \n");
    _models.pop_back();
  }
}

double strToDouble(std::string value)
{
  std::stringstream ss(value);
  double base = 0;
  double decimals = 0;

  char tmp1[10];
  ss.getline(tmp1, 10, ',');
  char tmp2[10];
  ss.getline(tmp2, 10);

  std::stringstream ssBase(tmp1);

  std::stringstream ssDecimals(tmp2);

  base = ssBase >> base ? base : 0; 
  decimals = ssDecimals >> decimals ? decimals : 0; 

  // std::cout << value <<  " base = "  << base << " decimals = " << decimals << std::endl;
  // std::cout << "value100 = " << base + decimals/100.0f << std::endl;
  // std::cout << "value10 = " << base + decimals/10.0f << std::endl;
  return base + decimals/10.0f;
}

void keyCallback(int key, int action)
{
  if (_engine->isMaster())
  {
    switch (key)
    {
      case 'W':
        arrowButtons[FORWARD] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
      break;
      case 'S':
        arrowButtons[BACKWARD] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
      break;
      case 'A':
        arrowButtons[LEFT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
      break;
      case 'D':
        arrowButtons[RIGHT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
      break;
    }
  }
}

void mouseButtonCallback(int key, int action)
{
  if (_engine->isMaster())
  {
    switch (key)
    {
      case SGCT_MOUSE_BUTTON_LEFT:
        _leftMousePressed = (action == SGCT_PRESS) ? true : false;
        sgct::Engine::getMousePos(_engine->getFocusedWindowIndex(), &_mouseClickXPos, &_mouseClickYPos);
      break;
    }
  }
}