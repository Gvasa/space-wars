#include <thread>
#include <mutex>
#include <iostream>
 
#include "sgct.h"
#include <sgct/SGCTSettings.h>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include "enet/enet.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
 
#include "Package.h"
 
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
 
void network();
std::mutex offMutex;
std::mutex posMutex;
std::mutex startMutex;
bool turnOfNetwork = false;
bool shouldStart = false;
glm::vec3 playerPositions[10];
int numberOfPlayers = 0;
int myPlayerNumber = 0;
osg::Geode* playerBoxes[10];
 
GLuint myLandscapeDisplayList = 0;
const int landscapeSize = 100;
const int numberOfPyramids = 150;
 
bool arrowButtons[6];
enum directions { FORWARD = 0, BACKWARD, LEFT, RIGHT, ROLL_RIGHT, ROLL_LEFT};
 
//to check if left mouse button is pressed
bool mouseLeftButton = false;
/* Holds the difference in position between when the left mouse button
    is pressed and when the mouse button is held. */
double mouseDx = 0.0;
double mouseDy = 0.0;
/* Stores the positions that will be compared to measure the difference. */
double mouseXPos = 0;
double mouseYPos = 0;
 
glm::vec3 position = glm::vec3( 0, 0, 0 );
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float speed = 3.0f;
float mouseSpeed = 0.5f;
 
sgct::SharedObject<glm::mat4> xform;
 
int main( int argc, char* argv[] )
{
  std::thread networkThread(network);
  while(true)
  {
    startMutex.lock();
    if (shouldStart)
    {
      startMutex.unlock();
      break;
    }
 
    startMutex.unlock();
  }

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
 
  sgct::SGCTSettings::instance()->setSwapInterval(0);
  // sgct::SGCTSettings::instance()->setFXAASubPixTrim(1/2);
 
 
  // Init the engine
  if( !gEngine->init() )
  {
    delete gEngine;
    return EXIT_FAILURE;
  }
  gEngine->getActiveWindowPtr()->setNumberOfAASamples(16);
  gEngine->setWireframe(false);
 
 
  // Main loop
  gEngine->render();
 
  // Clean up (de-allocate)
  glDeleteLists(myLandscapeDisplayList, 1);
  delete gEngine;
 
  offMutex.lock();
  turnOfNetwork = true;
  offMutex.unlock();
  networkThread.join();
 
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

  int size = 100;
  for(int x = -(size/2); x < (size/2); x++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(x,0,0), 0.05, 0.05, size);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable);
    mRootNode->addChild(gridShapesGeode);
  }

  for(int z = -(size/2); z < (size/2); z++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(0,0,z), size, 0.05, 0.05);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable);
    mRootNode->addChild(gridShapesGeode);
  }

  
  
 
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
 
  // mViewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(-2,2,-2), osg::Vec3d(0,0,0), osg::Vec3d(0,1,0));
 
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
 
    int width, height;
    width = gEngine->getActiveXResolution();
    height = gEngine->getActiveYResolution();
 
    sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos, &mouseYPos);
    mouseDx = mouseXPos - width/2;
    mouseDy = mouseYPos - height/2;
 
    sgct::Engine::setMousePos( gEngine->getFocusedWindowIndex(), width/2, height/2);
    
    float deltaTime = gEngine->getDt();
    horizontalAngle += mouseSpeed * deltaTime * float(width/2 - mouseXPos );
    verticalAngle   += mouseSpeed * deltaTime * float( height/2 - mouseYPos );
    
    glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle),
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle) );

    glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f/2.0f),
    0,
    cos(horizontalAngle - 3.14f/2.0f));

    glm::vec3 up = glm::cross( right, direction );

    if( arrowButtons[FORWARD] ){
      position += direction * deltaTime * speed;
 
    }
 
    if( arrowButtons[BACKWARD] ){
      position -= direction * deltaTime * speed;
 
    }
 
    if( arrowButtons[LEFT] ){
      position -= right * deltaTime * speed;
 
    }
 
    if( arrowButtons[RIGHT] ){
        position += right * deltaTime * speed;
 
    }
 
 
    osg::Vec3d vPos(position.x, position.y, position.z);
    osg::Vec3d vView(position.x + direction.x, position.y + direction.y, position.z + direction.z);
    osg::Vec3d vUp(up.x, up.y, up.z);
 
    mViewer->getCamera()->setViewMatrixAsLookAt(vPos, vView, vUp);
 
    // glm::mat4 result;
    // result = glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
    // //2. apply transformation
    // result *= (ViewMat *  glm::translate( glm::mat4(1.0f), pos ));
    // //1. transform user to coordinate system origin
    // result *= glm::translate( glm::mat4(1.0f), -sgct::Engine::getUserPtr()->getPos() );
 
    // xform.setVal( result );
 
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
  // for (int i = 0; i < numberOfPlayers; ++i)
  // {
  //   playerBoxes[i]->setCenter(osg::Vec3(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z));
  // }
  std::cout << "Number: " << myPlayerNumber << std::endl;
  for (int i = 0; i < numberOfPlayers; i++)
  {
    // playerBoxes[i] = new osg::Box( osg::Vec3(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z), 1.0f);
    // osg::ShapeDrawable* playerCubeDrawable = new osg::ShapeDrawable(playerBoxes[i]);
    // osg::Geode* playerShapesGeode = new osg::Geode();
    // playerShapesGeode->addDrawable(playerCubeDrawable);
    // mRootNode->addChild(playerShapesGeode);
    if (i != myPlayerNumber)
    {
      mRootNode->removeChild(playerBoxes[i]);

      osg::Sphere* gridCube = new osg::Sphere( osg::Vec3(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z), 1);
      osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
      playerBoxes[i] = new osg::Geode();
      playerBoxes[i]->addDrawable(gridCubeDrawable);
      mRootNode->addChild(playerBoxes[i]);
    }
    
  }

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
  if( gEngine->isMaster() ) {
      switch( key ){
 
      case SGCT_KEY_UP:
      case SGCT_KEY_W:
        arrowButtons[FORWARD] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
 
      case SGCT_KEY_DOWN:
      case SGCT_KEY_S:
        arrowButtons[BACKWARD] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
 
      case SGCT_KEY_LEFT:
      case SGCT_KEY_A:
        arrowButtons[LEFT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
 
      case SGCT_KEY_RIGHT:
      case SGCT_KEY_D:
        arrowButtons[RIGHT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
       
     
      case SGCT_KEY_Q:
        arrowButtons[ROLL_LEFT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
       
     
      case SGCT_KEY_E:
        arrowButtons[ROLL_RIGHT] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
        break;
      }
  }
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
 


void network()
{

  for (int i = 0; i < 10; ++i)
  {
    playerPositions[i] = glm::vec3(0,0,0);
  }

  ENetAddress address;
  ENetHost* client;
  ENetPeer* peer;
  std::string message;
  ENetEvent event;
  int eventStatus;
 
  if (enet_initialize () != 0)
  {
      std::cout << "Could not start ENet." << std::endl;
      return;
  }else
    std::cout << "Started ENet." << std::endl;
  atexit(enet_deinitialize);
 
  client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
 
  enet_address_set_host(&address, "192.168.43.158");
  char hej[10];
  enet_address_get_host(&address, hej, 10);
  // std::cout << hej << std::endl;
  address.port = 1234;
 
 
  peer = enet_host_connect(client, &address, 2, 0);
 
  while(true)
  {
    while (enet_host_service(client, &event, 10) > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
        std::cout << "We got a new connection " << event.peer->address.host << std::endl;
        break;
 
      case ENET_EVENT_TYPE_RECEIVE:
      {
          // std::cout << "Message from servv: ";
          //enet_peer_disconnect(peer, 3);
          char header[] = {((char*) event.packet->data)[0],
                            ((char*) event.packet->data)[1],
                            ((char*) event.packet->data)[2],
                            ((char*) event.packet->data)[3]};
          int* headerInt = (int*) header;
          // std::cout << *headerInt << std::endl;
 
          if (*headerInt == PLAYER_POSITION)
          {
            Package<PLAYER_POSITION_TYPE>* message = (Package<PLAYER_POSITION_TYPE>*) event.packet->data;
            // std::cout << "player: " << message->_player << std::endl;
            // std::cout << "x: " << message->_data.x << std::endl;
            // std::cout << "y: " << message->_data.y << std::endl;
            // std::cout << "z: " << message->_data.z << std::endl;
            playerPositions[message->_player] = message->_data;

          }else if (*headerInt == ASSIGN_PLAYER_NUMBER)
          {
            Package<int>* message = (Package<int>*) event.packet->data;
            // std::cout << "Player number: " << message->_data << std::endl;
            myPlayerNumber = message->_data;
          }else if (*headerInt == START_GAME)
          {
            Package<int>* message = (Package<int>*) event.packet->data;
            numberOfPlayers = message->_data;
            startMutex.lock();
            shouldStart = true;
            startMutex.unlock();
          }
         
          // int message = ((char*) event.packet->data)
          enet_packet_destroy(event.packet);
       
       
 
        break;
      }
      case ENET_EVENT_TYPE_DISCONNECT:
       
        std::cout << "Disconnected from serv: " << event.peer->data << std::endl;
        event.peer->data = NULL;
        break;
      }
    }
 
    posMutex.lock();
    glm::vec3 hej = position;
    posMutex.unlock();
    // std::cout << "Position sent: (" << hej.x << ", " << hej.y << ", " << hej.x << ")" << std::endl;
    ENetPacket* packet = enet_packet_create(&hej, sizeof(glm::vec3), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
 
    // std::cout << "ping: " << peer->roundTripTime << " ms" << std::endl;
    offMutex.lock();
    if (turnOfNetwork)
    {
      offMutex.unlock();
      std::cout << "Network recived shoudown command." << std::endl;
      break;
    }else
      offMutex.unlock();
  }
  std::cout << "Turning of network." << std::endl;
 
  enet_host_destroy(client);
}