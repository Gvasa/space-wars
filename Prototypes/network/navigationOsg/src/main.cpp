#include "sgct.h"
#include <iostream>
#include <string>
#include "enet/enet.h"
 
#include <thread>
#include <mutex>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <btBulletDynamicsCommon.h>
#include <osg/TextureCubeMap>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
 
#include "skybox.h"
#include "Package.h"
#include "NavigationPackage.h"
#include "set.cpp"

sgct::Engine * gEngine;

//Not using ref pointers enables
//more controlled termination
//and prevents segfault on Linux
osgViewer::Viewer * mViewer;
osg::ref_ptr<osg::Group> mRootNode;
osg::ref_ptr<osg::MatrixTransform> mSceneTrans;
osg::ref_ptr<osg::FrameStamp> mFrameStamp; //to sync osg animations across cluster

//callbacks
void myInitOGLFun();
void myPreSyncFun();
void myPostSyncPreDrawFun();
void myDrawFun();
void myEncodeFun();
void myDecodeFun();
void myCleanUpFun();
void keyCallback(int key, int action);
void printOsgMat(osg::Matrixd tmp);

//other functions
void initOSG();
void setupLightSource();

//variables to share across cluster
sgct::SharedDouble curr_time(0.0);
sgct::SharedDouble dist(-2.0);
sgct::SharedDouble sideways(0.0);
sgct::SharedBool wireframe(false);
sgct::SharedBool info(false);
sgct::SharedBool stats(false);
sgct::SharedBool takeScreenshot(false);
sgct::SharedBool light(true);

float rotationSpeed = 4.0f;
float rollSpeed = 50.0f;
float walkingSpeed = 10.0f;
float projectile_speed = 50.0f;

glm::mat4 result;

glm::vec3 view(0.0f, 0.0f, 1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 pos(0.0f, 0.0f, 0.0f);


sgct::SharedObject<glm::mat4> xform;


//other var
bool arrowButtons[6];
enum directions { FORWARD = 0, BACKWARD, LEFT, RIGHT, LEFT_MOUSE, LASER };

double mouseDx = 0.0;
double mouseDy = 0.0;

double mouseXPos = 0.0;
double mouseYPos = 0.0;

const int size = 1000;
glm::vec3 viewdir;
glm::vec3 projectilepos;
glm::mat4 ViewArray;

double checktime = 0.0;
double checktime2[size];

int checkchild;
int checklaser = 0;
int laserpos = 1;

int numchild = 202;
int i = 0;
int j = 0;

Set<glm::vec3> positions;
Set<glm::vec3> viewdirections;
Set<glm::mat4> viewarrays;

int main( int argc, char* argv[] )
{
	gEngine = new sgct::Engine( argc, argv );

	gEngine->setInitOGLFunction( myInitOGLFun );
	gEngine->setPreSyncFunction( myPreSyncFun );
	gEngine->setPostSyncPreDrawFunction( myPostSyncPreDrawFun );
	gEngine->setDrawFunction( myDrawFun );
	gEngine->setCleanUpFunction( myCleanUpFun );
	gEngine->setKeyboardCallbackFunction( keyCallback );

	for(int i=0; i<4; i++)
		arrowButtons[i] = false;

	if( !gEngine->init() )
	{
		delete gEngine;
		return EXIT_FAILURE;
	}

	sgct::SharedData::instance()->setEncodeFunction( myEncodeFun );
	sgct::SharedData::instance()->setDecodeFunction( myDecodeFun );

	// Main loop
	gEngine->render();

	// Clean up
	delete gEngine;

	// Exit program
	exit( EXIT_SUCCESS );
}

void myInitOGLFun()
{
	initOSG();

	osg::ref_ptr<osg::Node>            mModel;
	osg::ref_ptr<osg::MatrixTransform> mModelTrans;

	mSceneTrans		= new osg::MatrixTransform();
	mModelTrans		= new osg::MatrixTransform();

	//rotate osg coordinate system to match sgct
	mModelTrans->preMult(osg::Matrix::rotate(glm::radians(-90.0f),
                                            1.0f, 0.0f, 0.0f));

	mRootNode->addChild( mSceneTrans.get() );
	mSceneTrans->addChild( mModelTrans.get() );


 // SkyBox from OSG Cookbook
 osg::ref_ptr<osg::Geode> geode = new osg::Geode;
 gEngine->setNearAndFarClippingPlanes(0.1f,1000.0f);
 geode->addDrawable( new osg::ShapeDrawable(
       new osg::Sphere(osg::Vec3(), 950.0f)) );
 
  osg::ref_ptr<SkyBox> skybox = new SkyBox;
  skybox->getOrCreateStateSet()->setTextureAttributeAndModes( 0, new osg::TexGen );
  // skybox->setEnvironmentMap( 0,
  //      osgDB::readImageFile("BlueChecker.png"), osgDB::readImageFile("OrangeChecker.png"),
  //      osgDB::readImageFile("GreenChecker.png"), osgDB::readImageFile("YellowChecker.png"),
  //      osgDB::readImageFile("RedChecker.png"), osgDB::readImageFile("PurpleChecker.png") );
  skybox->setEnvironmentMap( 0,
       osgDB::readImageFile("stars.png"), osgDB::readImageFile("stars.png"),
       osgDB::readImageFile("stars.png"), osgDB::readImageFile("stars.png"),
       osgDB::readImageFile("stars.png"), osgDB::readImageFile("stars.png") );
  skybox->addChild( geode );
  mSceneTrans->addChild( skybox );

int size = 100;
  for(int x = -(size/2); x < (size/2); x++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(x,-1,0), 0.05, 0.05, size);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    gridCubeDrawable->computeBound();
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable); 
    mSceneTrans->addChild(gridShapesGeode);
  }
 
  for(int z = -(size/2); z < (size/2); z++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(0,-1,z), size, 0.05, 0.05);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable);
    mSceneTrans->addChild(gridShapesGeode);
  }

	setupLightSource();
}


void initOSG()
{
	mRootNode = new osg::Group();
	osg::Referenced::setThreadSafeReferenceCounting(true);

	// Create the osgViewer instance
	mViewer = new osgViewer::Viewer;

	// Create a time stamp instance
	mFrameStamp	= new osg::FrameStamp();

	//run single threaded when embedded
	mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// Set up osgViewer::GraphicsWindowEmbedded for this context
	osg::ref_ptr< ::osg::GraphicsContext::Traits > traits =
      new osg::GraphicsContext::Traits;

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
      new osgViewer::GraphicsWindowEmbedded(traits.get());

	mViewer->getCamera()->setGraphicsContext(graphicsWindow.get());

	//SGCT will handle the near and far planes
	mViewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
	mViewer->getCamera()->setClearColor( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f) );

	//disable osg from clearing the buffers that will be done by SGCT
	GLbitfield tmpMask = mViewer->getCamera()->getClearMask();
	mViewer->getCamera()->setClearMask(tmpMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));

	mViewer->setSceneData(mRootNode.get());
}

void myPreSyncFun()
{
	if( gEngine->isMaster() )
	{
		curr_time.setVal( sgct::Engine::getTime() );

		int width, height;
		width = gEngine->getActiveXResolution();
		height = gEngine->getActiveYResolution();

		sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos, &mouseYPos);
		mouseDx = mouseXPos - width/2;
		mouseDy = mouseYPos - height/2;

		sgct::Engine::setMousePos( gEngine->getFocusedWindowIndex(), width/2, height/2);

		static float vertRot = 0.0f;
		vertRot += (static_cast<float>(mouseDy) * rotationSpeed * static_cast<float>(gEngine->getDt()));

		if(vertRot > 180)
			vertRot = -180;
		if(vertRot < -180)
			vertRot = 180;

		static float panRot = 0.0f;

		if(vertRot < 90 && vertRot > -90)
			panRot += (static_cast<float>(mouseDx) * rotationSpeed * static_cast<float>(gEngine->getDt()));
		else
			panRot -= (static_cast<float>(mouseDx) * rotationSpeed * static_cast<float>(gEngine->getDt()));

		if(panRot > 180)
			panRot = -180;
		if(panRot < -180)
			panRot = 180;
		
		// static float rollRot = 0.0f;

		// std::cout << " panRot: " << panRot << std::endl;
		// std::cout << " vertRot: " << vertRot << std::endl;


		glm::mat4 ViewRotateX = glm::rotate(
			glm::mat4(1.0f),
			panRot,
			glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around the y-axis

		glm::mat4 ViewRotateY = glm::rotate(
			glm::mat4(1.0f),
			vertRot,
			glm::vec3(1.0f, 0.0f, 0.0f)); //rotation around the x-axis
			
		// glm::mat4 ViewRotateZ = glm::rotate(
		// 	glm::mat4(1.0f),
		// 	rollRot,
		// 	glm::vec3(0.0f, 0.0f, 1.0f)); //rotation around the z-axis
		

		glm::mat4 ViewMat = ViewRotateY * ViewRotateX;
		view = glm::inverse(glm::mat3(ViewMat)) * glm::vec3(0.0f, 0.0f, 1.0f);


		glm::vec3 right = glm::cross(view, up);

		if( arrowButtons[FORWARD] )
			pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);
		if( arrowButtons[BACKWARD] )
			pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);


		if( arrowButtons[LEFT] ){
			if(vertRot < 90 && vertRot > -90)
				pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
			else
				pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		}

		if( arrowButtons[RIGHT] ){
			if(vertRot < 90 && vertRot > -90)
				pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
			else
				pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		}


		if( arrowButtons[LEFT_MOUSE] && checktime+0.1 < sgct::Engine::getTime()){

			checktime = sgct::Engine::getTime();

			osg::MatrixTransform* trans = new osg::MatrixTransform;


			if(laserpos % 2 != 0){

				osg::Box* projectile = new osg::Box( osg::Vec3(-0.5f, 0.0f, 0.0f), 0.1f, 0.1f, 0.4f);
				osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
				projectileDrawable->computeBound();
				osg::Geode* projectileGeode = new osg::Geode();
				projectileGeode->addDrawable(projectileDrawable);
				trans->addChild(projectileGeode);

				projectilepos = glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f);

				glm::mat4 temp =  glm::translate( glm::mat4(1.0f), projectilepos)
								 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				laserpos++;

				trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));

			}
			else{

				osg::Box* projectile = new osg::Box( osg::Vec3(0.5f, 0.0f, 0.0f), 0.1f, 0.1f, 0.4f);
				osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
				projectileDrawable->computeBound();
				osg::Geode* projectileGeode = new osg::Geode();
				projectileGeode->addDrawable(projectileDrawable);
				trans->addChild(projectileGeode);
				

				projectilepos = glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f);

				glm::mat4 temp =  glm::translate( glm::mat4(1.0f), projectilepos)
								 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				laserpos++;

				trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));
			}

			
			mSceneTrans->addChild(trans);

			// checkchild = mSceneTrans->getChildIndex(trans);

			// for(int p = 202; p < mSceneTrans->getNumChildren()-1; p++){
			// 	std::cout << mSceneTrans->getNumChildren() << std::endl;
			// }

			viewdir = view;

			ViewArray = ViewMat;

			positions.add(projectilepos);
			viewdirections.add(viewdir);
			viewarrays.add(ViewArray);

			// std::cout << checkchild << std::endl;
			// std::cout << "i: " << i << std::endl;
			checktime2[i] = sgct::Engine::getTime();
			i++;
			if(i == size)
				i = 0;
		}


		Set<glm::vec3>::Node* temp1 = positions.head->next;
		Set<glm::vec3>::Node* temp2 = viewdirections.head->next;
		Set<glm::mat4>::Node* temp3 = viewarrays.head->next;

		for(int q = 202; q < mSceneTrans->getNumChildren(); q++){

				

				glm::mat4 tempen = glm::translate( glm::mat4(1.0f), temp1->pos)
								 		* glm::inverse(temp3->pos) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				mSceneTrans->getChild(q)->asTransform()->asMatrixTransform()->setMatrix(osg::Matrixd(glm::value_ptr(tempen)) );


				// std::cout << k << std::endl;
				// std::cout << mSceneTrans->getNumChildren()-203 << std::endl;

				temp1->pos += (-temp2->pos * projectile_speed * static_cast<float>(gEngine->getDt()) );

				temp1 = temp1->next;
				temp2 = temp2->next;
				temp3 = temp3->next;

		}


		if(checktime2[j] != 0.0 && checktime2[j]+10.0 < sgct::Engine::getTime()){
			mSceneTrans->removeChild(numchild, 1);

			// std::cout << "j: " << j << std::endl;
			checktime2[j] = 0.0;

			positions.remove();
			viewdirections.remove();
			viewarrays.remove();

			j++;
			if(j == size)
				j = 0;
	
			// checktime2 = sgct::Engine::getTime();
		}

		if(checklaser == 1){
			mSceneTrans->removeChild(mSceneTrans->getChild(mSceneTrans->getNumChildren()-1));
			checklaser = 0;
		}

		if( arrowButtons[LASER] && !arrowButtons[LEFT_MOUSE]){

			checktime = sgct::Engine::getTime();


			osg::Box* projectile = new osg::Box( osg::Vec3(0.0f, -0.3f, -250.0f), 0.1f, 0.1f, 500.0f);
			osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
			projectileDrawable->computeBound();
			osg::Geode* projectileGeode = new osg::Geode();
			projectileGeode->addDrawable(projectileDrawable);

			osg::MatrixTransform* trans = new osg::MatrixTransform;

			trans->addChild(projectileGeode);


			glm::mat4 temp =  glm::translate( glm::mat4(1.0f), glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f))
								 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

			trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));
			mSceneTrans->addChild(trans);

			checklaser = 1;

		}
		
		result = glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
		//2. apply transformation
		result *= (ViewMat *  glm::translate( glm::mat4(1.0f), pos ));
		//1. transform user to coordinate system origin
		result *= glm::translate( glm::mat4(1.0f), -sgct::Engine::getUserPtr()->getPos() );

		
		mSceneTrans->setMatrix(osg::Matrixd(glm::value_ptr(result)));


	}
}

void myPostSyncPreDrawFun()
{
	gEngine->setWireframe(wireframe.getVal());
	gEngine->setDisplayInfoVisibility(info.getVal());
	gEngine->setStatsGraphVisibility(stats.getVal());

	if( takeScreenshot.getVal() )
	{
		gEngine->takeScreenshot();
		takeScreenshot.setVal(false);
	}

	light.getVal() ? mRootNode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE) :
		mRootNode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

	//transform to scene transformation from configuration file
	mSceneTrans->postMult( osg::Matrix( glm::value_ptr( gEngine->getModelMatrix() ) ));

	//update the frame stamp in the viewer to sync all
	//time based events in osg
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

void myDrawFun()
{
	const int * curr_vp = gEngine->getActiveViewportPixelCoords();
	mViewer->getCamera()->setViewport(curr_vp[0], curr_vp[1], curr_vp[2], curr_vp[3]);
	mViewer->getCamera()->setProjectionMatrix( osg::Matrix( glm::value_ptr(gEngine->getActiveViewProjectionMatrix() ) ));

	mViewer->renderingTraversals();
}

void myEncodeFun()
{
	sgct::SharedData::instance()->writeDouble( &curr_time );

	sgct::SharedData::instance()->writeBool( &wireframe );
	sgct::SharedData::instance()->writeBool( &info );
	sgct::SharedData::instance()->writeBool( &stats );
	sgct::SharedData::instance()->writeBool( &takeScreenshot );
	sgct::SharedData::instance()->writeBool( &light );

}

void myDecodeFun()
{
	sgct::SharedData::instance()->readDouble( &curr_time );

	sgct::SharedData::instance()->readBool( &wireframe );
	sgct::SharedData::instance()->readBool( &info );
	sgct::SharedData::instance()->readBool( &stats );
	sgct::SharedData::instance()->readBool( &takeScreenshot );
	sgct::SharedData::instance()->readBool( &light );

}

void myCleanUpFun()
{
	sgct::MessageHandler::instance()->print("Cleaning up osg data...\n");
	delete mViewer;
	mViewer = NULL;
}

void keyCallback(int key, int action)
{
	if( gEngine->isMaster() )
	{
		switch( key )
		{
		case 'J':
			if(action == SGCT_PRESS)
				stats.toggle();
			break;

		case 'I':
			if(action == SGCT_PRESS)
				info.toggle();
			break;

		case 'L':
			if(action == SGCT_PRESS)
				light.toggle();
			break;

		case 'K':
			if(action == SGCT_PRESS)
				wireframe.toggle();
			break;

		case 'Q':
			if(action == SGCT_PRESS)
				gEngine->terminate();
			break;

		case 'P':
		case SGCT_KEY_F10:
			if(action == SGCT_PRESS)
				takeScreenshot.setVal( true );
			break;

		case SGCT_KEY_UP:
		case 'W':
			arrowButtons[FORWARD] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
			break;

		case SGCT_KEY_DOWN:
		case 'S':
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

		case SGCT_MOUSE_BUTTON_LEFT:
		case SGCT_KEY_F:
			arrowButtons[LEFT_MOUSE] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
			break;

		case SGCT_KEY_G:
			arrowButtons[LASER] = ((action == SGCT_REPEAT || action == SGCT_PRESS) ? true : false);
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


void printOsgMat(osg::Matrixd tmp)
{
	std::cout << std::endl;    
    std::cout << std::endl;
    std::cout << tmp(0,0) << " " << tmp(0,1) << " " << tmp(0,2) << " " << tmp(0,3) << std::endl; 
    std::cout << tmp(1,0) << " " << tmp(1,1) << " " << tmp(1,2) << " " << tmp(1,3) << std::endl;
    std::cout << tmp(2,0) << " " << tmp(2,1) << " " << tmp(2,2) << " " << tmp(2,3) << std::endl;
    std::cout << tmp(3,0) << " " << tmp(3,1) << " " << tmp(3,2) << " " << tmp(3,3) << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

}