#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <vector>

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
#include <osgFX/Scribe>
#include <osgFX/Cartoon>
#include <osgFX/SpecularHighlights>
#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbDynamics/RigidBody.h>

#include "Gui.h"
#include "Info.h"

//! Handles the games scene graph.
/*!
  Contains the OSG scene-graph and connects it to the SGCT window.  
*/

class Renderer
{
  public:
    //! Constructor that initializes the OSG-viewer and creates the root node.
    Renderer();

    //! Basic destructor that removes dynamically allocated resources.
    ~Renderer();
    
    //! Renders the current scene-graph.
    void render();

    //! Update that occurs before sgct sync. 
    /*!
      \param mousePos The current mouse position given by sgct.
    */
    void updatePreSync(double currentTime, int* mousePos);

    //! Update that occurs after sgct sync. 
    /*!
        Called from main loop to make sure that all sgct configuration is correct.
        \param currentTime The time currently given by sgct.
        \param frameNumber The framenumber currently given by sgct.
        \param modelMatrix The config matrix given by sgct.
    */
    void updatePostSync(double currentTime,  unsigned int frameNumber, glm::mat4 modelMatrix);

    //! Sets the projection matrix used for rendering.
    /*!
        This function should be called when the game starts or when the projection matix changes. 
        \param mat The projection matrix. Should be given by sgct.
    */
    void setProjectionMatrix(glm::mat4 mat);

    //! Sets the pixel coordinates used for rendering. 
    /*!
        The pixel coordinates are given by the sgct getActiveViewportPixelCoords() function.  
    */
    void setPixelCoords(int vp1, int vp2, int vp3, int vp4);
    //! Sets the scene transform used for rendering. 
    /*!
        The scene transform moves the world around the player and should be given by the physics engine. 
    */
    void setSceneTransform(glm::mat4 transform);

    //! Test functions that sets up a few test lights.
    void tempSetUpLight();

    //! Adds a node to the scen transform.
    void addNodeToScene(osg::Node* node);

    //! Update transform of node with index i.
    void updateNode(int i, glm::mat4 transform);
    
    //! Update transform of node with index i.
    void updateNode(int i, osg::Matrix transform);

    /**
     * @brief Create the skybox.
     */
    void setUpSkyBox();

    void setTranslationTransform(glm::mat4 translationMatrix);

    void setRotationTransform(glm::mat4 rotationMatrix);
  
  private:
    osgViewer::Viewer* _viewer; //!< The osg viewer connected to sgct.
    osg::ref_ptr<osg::Group> _root; //!< The root of the scene graph.
    osg::ref_ptr<osg::MatrixTransform> _sceneTransform; //!< The top transform of the scene graph that controls camera player movement.
    osg::ref_ptr<osg::Group> _sceneObjects;
    osg::ref_ptr<osg::MatrixTransform> _toBulletTransform; 
    osg::ref_ptr<osg::FrameStamp> _frameStamp; //!< Framestamp that needs to be synced between sgct and osg.

    osg::Matrix _projectionMatrix; //!< The projection matrox used for rendering.
    int* _pixelCoords;  //!< A four slot integer array that holds the current viewport coordinate. 

    std::vector<osg::MatrixTransform*> _transforms;

    Gui* _gui;

    int _mouseXpos;
    int _mouseYpos;

    int _playerSpeedGuiTextIndex = 0;
    int _playerVelocityGuiTextIndex = 0;
    int _playerAngularVelocityGuiIndex = 0;

    osg::ref_ptr<osg::MatrixTransform> _rotationTransform;
    osg::ref_ptr<osg::MatrixTransform> _translationTransform;
};

#endif