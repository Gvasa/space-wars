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

// Other functions.
void addModel(std::string fileName);
double strToDouble(std::string value);

sgct::SharedDouble curr_time(0.0);

std::vector<osg::ref_ptr<osg::Node>> _models;
std::vector<osg::ref_ptr<osg::MatrixTransform>> _modelTransforms;
std::vector<osg::Vec4d> _boundingBoxInfos;
int selectedModel = 0;
osg::Vec3d translation;
osg::Vec3d rotation;


int main(int argc, char* argv[])
{
  _engine = new sgct::Engine(argc, argv);

  _engine->setInitOGLFunction(init);
  _engine->setPreSyncFunction(preSync);
  _engine->setPostSyncPreDrawFunction(postSyncPreDraw);
  _engine->setDrawFunction(draw);
  _engine->setCleanUpFunction(cleanUp);
  _engine->setExternalControlCallback(clientCallback);

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
  light0->setAmbient( osg::Vec4( 0.3f, 0.3f, 0.3f, 1.0f ) );
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
  }
}

void postSyncPreDraw()
{
  _root->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

  // _sceneTransform->setMatrix(osg::Matrix::rotate( glm::radians(curr_time.getVal() * 8.0), 0.0, 1.0, 0.0));
  _sceneTransform->setMatrix(osg::Matrix::translate(0.0, -0.1, -2));
  _sceneTransform->postMult( osg::Matrix( glm::value_ptr(_engine->getModelMatrix())));

  if (_modelTransforms.size() > selectedModel)
  {
    double radius = _boundingBoxInfos.at(selectedModel).w();
    osg::Vec3d tmpVec(_boundingBoxInfos.at(selectedModel).x(), _boundingBoxInfos.at(selectedModel).y(), _boundingBoxInfos.at(selectedModel).z());
    _modelTransforms.at(selectedModel)->setMatrix(osg::Matrix::rotate(glm::radians(-90.0f), 1.0f, 0.0f, 0.0f));
    _modelTransforms.at(selectedModel)->postMult(osg::Matrix::translate( -tmpVec ) );
    _modelTransforms.at(selectedModel)->postMult(osg::Matrix::scale( 1.0f/radius, 1.0f/radius, 1.0f/radius ));

    _modelTransforms.at(selectedModel)->postMult(osg::Matrix::translate(translation));
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

    }else if (strncmp(receivedChars, "positionX", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.x() = strToDouble(tmp);
      // std::cout << translation.x() << std::endl;
    }else if (strncmp(receivedChars, "positionY", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.y() = strToDouble(tmp);
    }else if (strncmp(receivedChars, "positionZ", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      translation.z() = strToDouble(tmp);
    }else if (strncmp(receivedChars, "rotationX", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.x() = strToDouble(tmp);
    }else if (strncmp(receivedChars, "rotationY", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.y() = strToDouble(tmp);
    }else if (strncmp(receivedChars, "rotationZ", 9) == 0)
    {
      std::string tmp(receivedChars + 10, receivedChars + size);
      rotation.z() = strToDouble(tmp);
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
    _modelTransforms.back()->setMatrix(osg::Matrix::rotate(glm::radians(-90.0f), 1.0f, 0.0f, 0.0f));

    _modelTransforms.back()->addChild(_models.back().get());

    osg::ComputeBoundsVisitor cbv;
    osg::BoundingBox &bb(cbv.getBoundingBox());
    
    _modelTransforms.back()->accept( cbv );

    osg::Vec3f tmpVec;
    tmpVec = bb.center();

    _boundingBoxInfos.push_back(osg::Vec4d(tmpVec, bb.radius()));

    _modelTransforms.back()->postMult(osg::Matrix::translate( -tmpVec ) );
    _modelTransforms.back()->postMult(osg::Matrix::scale( 1.0f/bb.radius(), 1.0f/bb.radius(), 1.0f/bb.radius() ));
    _sceneTransform->addChild(_modelTransforms.back());

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