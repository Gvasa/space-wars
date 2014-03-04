#include <thread>
#include <mutex>
#include <iostream>

#include "sgct.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "enet/enet.h"

#include "Package.h"

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
void drawColorCube(float r);

// Network
void network();
std::mutex offMutex;
std::mutex posMutex;
bool turnOfNetwork = false;

float rotationSpeed = 0.3f;
float walkingSpeed = 2.5f;

GLuint myLandscapeDisplayList = 0;
const int landscapeSize = 50;
const int numberOfPyramids = 150;

bool arrowButtons[4];
enum directions { FORWARD = 0, BACKWARD, LEFT, RIGHT };

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
	std::thread networkThread(network);
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

	offMutex.lock();
	turnOfNetwork = true;
	offMutex.unlock();
	networkThread.join();

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
		// if( mouseLeftButton )
		// {
			//double tmpYPos;
			//get the mouse pos from first window
			sgct::Engine::getMousePos( gEngine->getFocusedWindowIndex(), &mouseXPos[0], &mouseYPos[0] );
			// mouseDx = mouseXPos[0] - mouseXPos[1];
			// mouseDy = mouseYPos[0] - mouseYPos[1];

			mouseDx = mouseXPos[0] - 960/2;
			mouseDy = mouseYPos[0] - 540/2;
		// }
		// else
		// {
		// 	mouseDx = 0.0;
		// 	mouseDy = 0.0;
		// }


		sgct::Engine::setMousePos( gEngine->getFocusedWindowIndex(), 960/2, 540/2);

		static float panRot = 0.0f;
		panRot += (static_cast<float>(mouseDx) * rotationSpeed * static_cast<float>(gEngine->getDt()));
		static float vertRot = 0.0f;
		vertRot += (static_cast<float>(mouseDy) * rotationSpeed * static_cast<float>(gEngine->getDt()));


		glm::mat4 ViewRotateX = glm::rotate(
			glm::mat4(1.0f),
			panRot,
			glm::vec3(0.0f, 1.0f, 0.0f)); //rotation around the y-axis

		glm::mat4 ViewRotateY = glm::rotate(
			glm::mat4(1.0f),
			vertRot,
			glm::vec3(1.0f, 0.0f, 0.0f)); //rotation around the x-axis

		view = glm::inverse(glm::mat3(ViewRotateX)) * glm::vec3(0.0f, 0.0f, 1.0f);


		glm::vec3 right = glm::cross(view, up);

		posMutex.lock();
		if( arrowButtons[FORWARD] )
			pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);
		if( arrowButtons[BACKWARD] )
			pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * view);
		if( arrowButtons[LEFT] )
			pos -= (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		if( arrowButtons[RIGHT] )
			pos += (walkingSpeed * static_cast<float>(gEngine->getDt()) * right);
		posMutex.unlock();
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
		// std::cout << "Position: (" << pos.x << ", " << pos.y << ", " << pos.x << ")" << std::endl;
		//3. transform user back to original position
		glm::mat4 result;
		result = glm::translate( glm::mat4(1.0f), sgct::Engine::getUserPtr()->getPos() );
		//2. apply transformation
		result *= (ViewRotateY * ViewRotateX *  glm::translate( glm::mat4(1.0f), pos ));
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


void drawColorCube(float r)
{
  glBegin(GL_QUADS);
    // +X face
    glColor3f(1,1,0);
    glNormal3f(1,0,0);
    glVertex3f(r,r,-r);
    glVertex3f(r,r,r);
    glVertex3f(r,-r,r);
    glVertex3f(r,-r,-r);
    // -X face
    glColor3f(0,0,1);
    glNormal3f(-1,0,0);
    glVertex3f(-r,r,r);
    glVertex3f(-r,r,-r);
    glVertex3f(-r,-r,-r);
    glVertex3f(-r,-r,r);
    // +Y face
    glColor3f(1,0,1);
    glNormal3f(0,1,0);
    glVertex3f(-r,r,r);
    glVertex3f(r,r,r);
    glVertex3f(r,r,-r);
    glVertex3f(-r,r,-r);
    // -Y face
    glColor3f(0,1,0);
    glNormal3f(0,-1,0);
    glVertex3f(-r,-r,r);
    glVertex3f(-r,-r,-r);
    glVertex3f(r,-r,-r);
    glVertex3f(r,-r,r);
    // +Z face
    glColor3f(0,1,1);
    glNormal3f(0,0,1);
    glVertex3f(-r,r,r);
    glVertex3f(-r,-r,r);
    glVertex3f(r,-r,r);
    glVertex3f(r,r,r);
    // -Z face
    glColor3f(1,0,0);
    glNormal3f(0,0,-1);
    glVertex3f(-r,r,-r);
    glVertex3f(r,r,-r);
    glVertex3f(r,-r,-r);
    glVertex3f(-r,-r,-r);
  glEnd();
}

void network()
{
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

  enet_address_set_host(&address, "localhost");
  char hej[10];
  enet_address_get_host(&address, hej, 10);
  std::cout << hej << std::endl;
  address.port = 1234;
  

  peer = enet_host_connect(client, &address, 2, 0);

  while(true)
  {
    while (enet_host_service(client, &event, 1000) > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
        std::cout << "We got a new connection " << event.peer->address.host << std::endl;
        break;

      case ENET_EVENT_TYPE_RECEIVE:
      {
          std::cout << "Message from servv: ";
          //enet_peer_disconnect(peer, 3);
          char header[] = {((char*) event.packet->data)[0],
                            ((char*) event.packet->data)[1],
                            ((char*) event.packet->data)[2],
                            ((char*) event.packet->data)[3]};
          int* headerInt = (int*) header;
          std::cout << *headerInt << std::endl;

          if (*headerInt == PLAYER_POSITION)
          {
            Package<PLAYER_POSITION_TYPE>* message = (Package<PLAYER_POSITION_TYPE>*) event.packet->data;
            std::cout << "player: " << message->_player << std::endl;
            std::cout << "x: " << message->_data.x << std::endl;
            std::cout << "y: " << message->_data.y << std::endl;
            std::cout << "z: " << message->_data.z << std::endl;
          }else if (*headerInt == ASSIGN_PLAYER_NUMBER)
          {
            Package<int>* message = (Package<int>*) event.packet->data;
            std::cout << "Player number: " << message->_data << std::endl;
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
    glm::vec3 hej = pos;
    posMutex.unlock();
    // std::cout << "Position sent: (" << hej.x << ", " << hej.y << ", " << hej.x << ")" << std::endl;
    ENetPacket* packet = enet_packet_create(&hej, sizeof(glm::vec3), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);

    std::cout << "ping: " << peer->roundTripTime << " ms" << std::endl;
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