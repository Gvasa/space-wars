#include "sgct.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

sgct::Engine * gEngine;

void myDrawFun();
void myPreSyncFun();
void myInitOGLFun();
void myEncodeFun();
void myDecodeFun();
//input callbacks
void keyCallback(int key, int action);
void mouseButtonCallback(int button, int action);

void drawXZGrid(int size, float yPos);
void drawPyramid(float width);

float rotationSpeed = 1.0f;
float rollSpeed = 50.0f;
float walkingSpeed = 2.5f;

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
double mouseXPos[] = { 0.0, 0.0 };
double mouseYPos[] = { 0.0, 0.0 };

glm::vec3 view(0.0f, 0.0f, 1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 pos(0.0f, 0.0f, 0.0f);

sgct::SharedObject<glm::mat4> xform;

int main( int argc, char* argv[] )
{
	gEngine = new sgct::Engine( argc, argv );

	gEngine->setInitOGLFunction( myInitOGLFun );
	gEngine->setDrawFunction( myDrawFun );
	gEngine->setPreSyncFunction( myPreSyncFun );
	gEngine->setKeyboardCallbackFunction( keyCallback );
	gEngine->setMouseButtonCallbackFunction( mouseButtonCallback );
	gEngine->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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
	glDeleteLists(myLandscapeDisplayList, 1);
	delete gEngine;

	// Exit program
	exit( EXIT_SUCCESS );
}

void myInitOGLFun()
{
	//create and compile display list
	myLandscapeDisplayList = glGenLists(1);
	glNewList(myLandscapeDisplayList, GL_COMPILE);

	drawXZGrid(landscapeSize, -1.5f);

	//pick a seed for the random function (must be same on all nodes)
	srand(9745);
	for(int i=0; i<numberOfPyramids; i++)
	{
		float xPos = static_cast<float>(rand()%landscapeSize - landscapeSize/2);
		float zPos = static_cast<float>(rand()%landscapeSize - landscapeSize/2);

		glPushMatrix();
		glTranslatef(xPos, -1.5f, zPos);
		drawPyramid(0.6f);
		glPopMatrix();
	}

	glEndList();
}

void myPreSyncFun()
{
	if( gEngine->isMaster() )
	{
		int width, height;
		width = gEngine->getActiveXResolution();
		height = gEngine->getActiveYResolution();

		//std::cout << "Bredd: " << width << " Höjd: " << height << std::endl;

		// if( mouseLeftButton )
		// {
			//double tmpYPos;
			//get the mouse pos from first window
			sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos[0], &mouseYPos[0] );
			// mouseDx = mouseXPos[0] - mouseXPos[1];
			// mouseDy = mouseYPos[0] - mouseYPos[1];

			mouseDx = mouseXPos[0] - width/2; //960
			mouseDy = mouseYPos[0] - height/2; //540
		// }
		// else
		// {
		// 	mouseDx = 0.0;
		// 	mouseDy = 0.0;
		// }


		sgct::Engine::setMousePos( gEngine->getFocusedWindowIndex(), width/2, height/2); //960, 540

		static float panRot = 0.0f;
		panRot += (static_cast<float>(mouseDx) * rotationSpeed * static_cast<float>(gEngine->getDt()));
		static float vertRot = 0.0f;
		vertRot += (static_cast<float>(mouseDy) * rotationSpeed * static_cast<float>(gEngine->getDt()));
		
		static float rollRot = 0.0f;
		if( arrowButtons[ROLL_RIGHT] )
			rollRot += (rollSpeed * static_cast<float>(gEngine->getDt()));
		if( arrowButtons[ROLL_LEFT] )
			rollRot -= (rollSpeed * static_cast<float>(gEngine->getDt()));


		glm::mat4 ViewRotateX = glm::rotate(
			glm::mat4(1.0f),
			panRot,
			glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around the y-axis

		glm::mat4 ViewRotateY = glm::rotate(
			glm::mat4(1.0f),
			vertRot,
			glm::vec3(1.0f, 0.0f, 0.0f)); //rotation around the x-axis
			
		glm::mat4 ViewRotateZ = glm::rotate(
			glm::mat4(1.0f),
			rollRot,
			glm::vec3(0.0f, 0.0f, 1.0f));
			
			
		glm::mat4 ViewMat = ViewRotateY * ViewRotateX;	
		//glm::mat4 ViewMat = ViewRotateY * ViewRotateX;
		view = glm::inverse(glm::mat3(ViewMat)) * glm::vec3(0.0f, 0.0f, 1.0f);


		glm::vec3 right = glm::cross(view, up);

		if( arrowButtons[FORWARD] )
			pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);
		if( arrowButtons[BACKWARD] )
			pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);
		if( arrowButtons[LEFT] )
			pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		if( arrowButtons[RIGHT] )
			pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		

		/*
			To get a first person camera, the world needs
			to be transformed around the users head.

			This is done by:
			1, Transform the user to coordinate system origin
			2, Apply transformation
			3, Transform the user back to original position

			However, mathwise this process need to be reversed
			due to the matrix multiplication order.
		*/

		//3. transform user back to original position
		glm::mat4 result;
		result = glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
		//2. apply transformation
		result *= (ViewMat *  glm::translate( glm::mat4(1.0f), pos ));
		//1. transform user to coordinate system origin
		result *= glm::translate( glm::mat4(1.0f), -sgct::Engine::getUserPtr()->getPos() );

		xform.setVal( result );


	}
}

void myDrawFun()
{
	glMultMatrixf(glm::value_ptr(xform.getVal()));
	glCallList(myLandscapeDisplayList);
}

void myEncodeFun()
{
	sgct::SharedData::instance()->writeObj( &xform );
}

void myDecodeFun()
{
	sgct::SharedData::instance()->readObj( &xform );
}

void keyCallback(int key, int action)
{
	if( gEngine->isMaster() )
	{
		switch( key )
		{
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

void mouseButtonCallback(int button, int action)
{
	if( gEngine->isMaster() )
	{
		switch( button )
		{
		case SGCT_MOUSE_BUTTON_LEFT:
			mouseLeftButton = (action == SGCT_PRESS ? true : false);
			//double tmpYPos;
			//set refPos
			sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos[1], &mouseYPos[1] );
			break;
		}
	}
}

void drawXZGrid(int size, float yPos)
{
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.0f, yPos, 0.0f);

	glLineWidth(3.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);

	glBegin( GL_LINES );
	for(int x = -(size/2); x < (size/2); x++)
	{
		glVertex3i(x, 0, -(size/2));
		glVertex3i(x, 0, (size/2));
	}

	for(int z = -(size/2); z < (size/2); z++)
	{
		glVertex3i(-(size/2), 0, z);
		glVertex3i((size/2), 0, z);
	}
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();
}

void drawPyramid(float width)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//disable depth sorting to avoid flickering
	glDisable(GL_DEPTH_TEST);

	glColor4f(1.0f, 0.0f, 0.5f, 0.6f);

	glBegin(GL_TRIANGLE_FAN);
	//draw top
	glVertex3f(0.0f, 2.0f, 0.0f);

	//draw sides
	glVertex3f(-width/2.0f, 0.0f, -width/2.0f);
	glVertex3f(-width/2.0f, 0.0f, width/2.0f);
	glVertex3f(width/2.0f, 0.0f, width/2.0f);
	glVertex3f(width/2.0f, 0.0f, -width/2.0f);
	glVertex3f(-width/2.0f, 0.0f, -width/2.0f);

	glEnd();

	//enhance the pyramids with lines in the edges
	glLineWidth(2.0f);
	glColor4f(1.0f, 0.0f, 0.5f, 0.8f);

	glBegin(GL_LINE_LOOP);
	glVertex3f(-width/2.0f, 0.0f, -width/2.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(-width/2.0f, 0.0f, width/2.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(-width/2.0f, 0.0f, width/2.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(width/2.0f, 0.0f, width/2.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(width/2.0f, 0.0f, width/2.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(width/2.0f, 0.0f, -width/2.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(width/2.0f, 0.0f, -width/2.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glVertex3f(-width/2.0f, 0.0f, -width/2.0f);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
