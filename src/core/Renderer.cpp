#include "Renderer.h"

Renderer::Renderer()
{
  _pixelCoords = new int[4];
  _root = new osg::Group();
  osg::Referenced::setThreadSafeReferenceCounting(true);
  _frameStamp = new osg::FrameStamp();

  _viewer = new osgViewer::Viewer();
  _viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

  osg::ref_ptr<::osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
      new osgViewer::GraphicsWindowEmbedded(traits.get());

  _viewer->getCamera()->setGraphicsContext(graphicsWindow.get());

  _viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR); 
  _viewer->getCamera()->setClearColor(osg::Vec4( 0.0f, 0.0f, 0.0f, 0.0f));

  GLbitfield clearMask = _viewer->getCamera()->getClearMask();
  _viewer->getCamera()->setClearMask(clearMask & (~(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));

  _viewer->setSceneData(_root.get());

  _sceneTransform = new osg::MatrixTransform();

  osg::ref_ptr<osgFX::Cartoon> cell_fx = new osgFX::Cartoon;
  osg::ref_ptr<osgFX::SpecularHighlights> specular_fx = new osgFX::SpecularHighlights;
  
  specular_fx->addChild(cell_fx.get());
  cell_fx->addChild(_sceneTransform.get());

  _root->addChild(specular_fx);

  _gui = new Gui();
  _gui->addGuiObject(724,244,350,100,"assets/gui/hpskold.png");
  _gui->addGuiObject(423,360,960,900,"assets/gui/character.png");
  _gui->addText(50, 1550, 1000, "Game of Domes\nAlpha", "C:/Windows/Fonts/impact.ttf");
  _root->addChild(_gui);

  _playerSpeedGuiTextIndex = _gui->addText(50, 10, 800, "0", "C:/Windows/Fonts/impact.ttf");
  _playerVelocityGuiTextIndex = _gui->addText(50, 10, 600, "0", "C:/Windows/Fonts/impact.ttf");

  int size = 1000;
  for(int x = -(size/2); x < (size/2); x++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(x,-1,0), 0.06, 0.06, size);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    gridCubeDrawable->computeBound();
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable);
    _sceneTransform->addChild(gridShapesGeode);
  }
 
  for(int z = -(size/2); z < (size/2); z++)
  {
    osg::Box* gridCube = new osg::Box( osg::Vec3(0,-1,z), size, 0.06, 0.06);
    osg::ShapeDrawable* gridCubeDrawable = new osg::ShapeDrawable(gridCube);
    osg::Geode* gridShapesGeode = new osg::Geode();
    gridShapesGeode->addDrawable(gridCubeDrawable);
    _sceneTransform->addChild(gridShapesGeode);
  }

  _root->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

  tempSetUpLight();
}

Renderer::~Renderer()
{
  delete _viewer;
  delete _pixelCoords;
}

void Renderer::render()
{
  _viewer->getCamera()->setViewport(_pixelCoords[0], _pixelCoords[1], _pixelCoords[2], _pixelCoords[3]);
  // std::cout << _pixelCoords[0] << " " << _pixelCoords[1] << " " << _pixelCoords[2] << " "  << _pixelCoords[3] << std::endl;
  _viewer->getCamera()->setProjectionMatrix(_projectionMatrix);

  _viewer->renderingTraversals();

}

void Renderer::updatePreSync(double currentTime, int* mousePos)
{
  _mouseXpos = mousePos[0];
  _mouseYpos = mousePos[1];
}

void Renderer::updatePostSync(double currentTime, unsigned int frameNumber, glm::mat4 modelMatrix)
{
  _gui->update(_mouseXpos, _mouseYpos);
  _gui->changeText(_playerSpeedGuiTextIndex, std::to_string(Info::getPlayerSpeed()));

  std::string playerVelocity = std::to_string(Info::getPlayerLinearVelocity().x) + "\n" + std::to_string(Info::getPlayerLinearVelocity().y) + "\n" + std::to_string(Info::getPlayerLinearVelocity().z) + "\n"; 
  _gui->changeText(_playerVelocityGuiTextIndex, playerVelocity);

  _sceneTransform->postMult(osg::Matrix(glm::value_ptr(modelMatrix)));

  _frameStamp->setFrameNumber(frameNumber);
  _frameStamp->setReferenceTime(currentTime);
  _frameStamp->setSimulationTime(currentTime);
  _viewer->setFrameStamp(_frameStamp.get());
  _viewer->advance(currentTime);

  if (!_viewer->done())
  {
    _viewer->eventTraversal();
    _viewer->updateTraversal();
  }
}

void Renderer::setProjectionMatrix(glm::mat4 mat)
{
  _projectionMatrix = osg::Matrix(glm::value_ptr(mat));
}

void Renderer::setPixelCoords(int vp1, int vp2, int vp3, int vp4)
{
  _pixelCoords[0] = vp1;
  _pixelCoords[1] = vp2;
  _pixelCoords[2] = vp3;
  _pixelCoords[3] = vp4;
}

void Renderer::setSceneTransform(glm::mat4 transform)
{
  _sceneTransform->setMatrix(osg::Matrixd(glm::value_ptr(transform)));
}

void Renderer::addNodeToScene(osg::Node* node)
{
  _transforms.push_back(new osg::MatrixTransform());
  _transforms.back()->addChild(node);

  _sceneTransform->addChild(_transforms.back());
}

void Renderer::updateNode(int i, glm::mat4 transform)
{
  _transforms[i]->setMatrix(osg::Matrix(glm::value_ptr(transform)));
}

void Renderer::updateNode(int i, osg::Matrix transform)
{
  _transforms[i]->setMatrix(transform);
}

void Renderer::tempSetUpLight()
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
}