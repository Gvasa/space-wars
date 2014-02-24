#include "sgct.h"
#include <sgct/SGCTSettings.h>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

sgct::Engine * gEngine;

void myInitOGLFun();
void myDrawFun();
void myPreSyncFun();
void myPostSyncPreDrawFun();
void myEncodeFun();
void myDecodeFun();
void myCleanUpFun();
void keyCallback(int key, int action);
void setupLightSource();

void initOSG();

sgct::SharedDouble curr_time(0.0);

osgViewer::Viewer * mViewer;
osg::ref_ptr<osg::Group> mRootNode;
osg::ref_ptr<osg::FrameStamp> mFrameStamp;

int main( int argc, char* argv[] )
{

  // Allocate
  gEngine = new sgct::Engine( argc, argv );

  // Bind your functions
  gEngine->setInitOGLFunction( myInitOGLFun );
  gEngine->setPreSyncFunction( myPreSyncFun );
  gEngine->setPostSyncPreDrawFunction( myPostSyncPreDrawFun );
  gEngine->setDrawFunction( myDrawFun );
  gEngine->setCleanUpFunction( myCleanUpFun );
  gEngine->setKeyboardCallbackFunction( keyCallback );
  sgct::SharedData::instance()->setEncodeFunction(myEncodeFun);
  sgct::SharedData::instance()->setDecodeFunction(myDecodeFun);

  // Init the engine
  if( !gEngine->init() )
  {
    delete gEngine;
    return EXIT_FAILURE;
  }

  // Main loop
  gEngine->render();

  // Clean up (de-allocate)
  delete gEngine;

  // Exit program
  exit( EXIT_SUCCESS );
}

void myInitOGLFun()
{
  initOSG();

  osg::Box* unitCube = new osg::Box( osg::Vec3(0,0,0), 1.0f);
  osg::ShapeDrawable* unitCubeDrawable = new osg::ShapeDrawable(unitCube);
  osg::Geode* basicShapesGeode = new osg::Geode();
  basicShapesGeode->addDrawable(unitCubeDrawable);
  mRootNode->addChild(basicShapesGeode);
  basicShapesGeode->getOrCreateStateSet()->setMode( GL_CULL_FACE,
      osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

  setupLightSource();

}

void initOSG()
{
  mRootNode = new osg::Group();
  osg::Referenced::setThreadSafeReferenceCounting(true);

  mViewer = new osgViewer::Viewer;
  mFrameStamp = new osg::FrameStamp();

  mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  osg::ref_ptr< ::osg::GraphicsContext::Traits > traits =
      new osg::GraphicsContext::Traits;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
      new osgViewer::GraphicsWindowEmbedded(traits.get());

  mViewer->getCamera()->setGraphicsContext(graphicsWindow.get());

  mViewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
  mViewer->getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f) );

  GLbitfield tmpMask = mViewer->getCamera()->getClearMask();
  mViewer->getCamera()->setClearMask(tmpMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));

  mViewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(-2,2,-2), osg::Vec3d(0,0,0), osg::Vec3d(0,1,0));

  mViewer->setSceneData(mRootNode.get());

}

void myDrawFun()
{
  const int * curr_vp = gEngine->getActiveViewportPixelCoords();
  mViewer->getCamera()->setViewport(curr_vp[0], curr_vp[1], curr_vp[2], curr_vp[3]);
  mViewer->getCamera()->setProjectionMatrix( osg::Matrix( glm::value_ptr(gEngine->getActiveViewProjectionMatrix() ) ));
  mViewer->renderingTraversals();
}

void myPreSyncFun()
{
  //set the time only on the master
  if( gEngine->isMaster() )
  {
    //get the time in seconds
    curr_time.setVal(sgct::Engine::getTime());
  }
}

void myEncodeFun()
{
  sgct::SharedData::instance()->writeDouble( &curr_time );
}

void myDecodeFun()
{
  sgct::SharedData::instance()->readDouble( &curr_time );
}

void myPostSyncPreDrawFun()
{
  mRootNode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

  mFrameStamp->setFrameNumber( gEngine->getCurrentFrameNumber() );
  mFrameStamp->setReferenceTime( curr_time.getVal() );
  mFrameStamp->setSimulationTime( curr_time.getVal() );
  mViewer->setFrameStamp( mFrameStamp.get() );
  mViewer->advance( curr_time.getVal() ); //update

  //traverse if there are any tasks to do
  if (!mViewer->done())
  {
    mViewer->eventTraversal();
    //update travelsal needed for pagelod object like terrain data etc.
    mViewer->updateTraversal();
  }

}

void myCleanUpFun()
{

}

void keyCallback(int key, int action)
{

}

void setupLightSource()
{
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
  lightSource0->setStateSetModes( *(mRootNode->getOrCreateStateSet()), osg::StateAttribute::ON );

  light1->setLightNum( 1 );
  light1->setPosition( osg::Vec4( -5.0f, -2.0f, 10.0f, 1.0f ) );
  light1->setAmbient( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
  light1->setDiffuse( osg::Vec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
  light1->setSpecular( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
  light1->setConstantAttenuation( 1.0f );

  lightSource1->setLight( light1 );
    lightSource1->setLocalStateSetModes( osg::StateAttribute::ON );
  lightSource1->setStateSetModes( *(mRootNode->getOrCreateStateSet()), osg::StateAttribute::ON );

  mRootNode->addChild( lightSource0 );
  mRootNode->addChild( lightSource1 );
}