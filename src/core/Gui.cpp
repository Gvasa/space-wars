#include "Gui.h"

Gui::Gui(int width, int height)
: _width(width), _height(height)
{
// A geometry node for our HUD:
	osg::Geode* HUDGeode = new osg::Geode();

	// Projection node for defining view frustrum for HUD:
	osg::Projection* HUDProjectionMatrix = new osg::Projection;

	// Initialize the projection matrix for viewing everything we
	// will add as descendants of this node. Use screen coordinates
	// to define the horizontal and vertical extent of the projection
	// matrix. Positions described under this node will equate to
	// pixel coordinates.
	//HUDProjectionMatrix->setMatrix(osg::Matrix::ortho2D(0,1024,0,768));
	HUDProjectionMatrix->setMatrix(osg::Matrix::ortho2D(0, _width, 0, _height));

	// For the HUD model view matrix use an identity matrix:
	_modelViewMatrix = new osg::MatrixTransform;
	_modelViewMatrix->setMatrix(osg::Matrix::identity());
	 

	// Make sure the model view matrix is not affected by any transforms
	// above it in the scene graph:
	_modelViewMatrix->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	// Add the HUD projection matrix as a child of the root node
	// and the HUD model view matrix as a child of the projection matrix
	// Anything under this node will be viewed using this projection matrix
	// and positioned with this model view matrix.
	this->addChild(HUDProjectionMatrix);
	
	HUDProjectionMatrix->addChild(_modelViewMatrix);

	// Add the Geometry node to contain HUD geometry as a child of the
	// HUD model view matrix.
	crosshairTransform = new osg::MatrixTransform();

	_modelViewMatrix->addChild( crosshairTransform );
	crosshairTransform->addChild(HUDGeode);
	
	// Set up geometry for the HUD and add it to the HUD
	osg::Geometry* HUD_CrosshairGeometry = new osg::Geometry();

	//Size of texture in pixels
	float size = 100;

	float cx = (width - size) / 2;
	float cy = (height  - size) / 2;
	
	//Draw edges of crosshair objekt
	osg::Vec3Array* HUD_CrosshairVertices = new osg::Vec3Array;
	HUD_CrosshairVertices->push_back( osg::Vec3( cx     , cy     , -1) );
	HUD_CrosshairVertices->push_back( osg::Vec3( cx+size, cy     , -1) );
	HUD_CrosshairVertices->push_back( osg::Vec3( cx+size, cy+size, -1) );
	HUD_CrosshairVertices->push_back( osg::Vec3( cx     , cy+size, -1) );

	osg::DrawElementsUInt* HUD_CrosshairIndices =
		new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
	HUD_CrosshairIndices->push_back(0);
	HUD_CrosshairIndices->push_back(1);
	HUD_CrosshairIndices->push_back(2);
	HUD_CrosshairIndices->push_back(3);

	//Choose transparens
	float transparens = 1.0;
	osg::Vec4Array* HUDcolors = new osg::Vec4Array;
	HUDcolors->push_back( osg::Vec4(1,1,1, transparens) );

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);

	HUD_CrosshairGeometry->setTexCoordArray(0,texcoords);
	osg::Texture2D* HUDTexture = new osg::Texture2D;
	HUDTexture->setDataVariance(osg::Object::DYNAMIC);
	osg::Image* hudImage;
	hudImage = osgDB::readImageFile("assets/gui/crosshair.png");
	HUDTexture->setImage(hudImage);

	HUD_CrosshairGeometry->addPrimitiveSet( HUD_CrosshairIndices );
	HUD_CrosshairGeometry->setVertexArray( HUD_CrosshairVertices );
	HUD_CrosshairGeometry->setColorArray( HUDcolors );
	HUD_CrosshairGeometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	
	HUDGeode->addDrawable(HUD_CrosshairGeometry);

	// Create and set up a state set using the texture from above:
	osg::StateSet* HUDStateSet = new osg::StateSet();
	HUDGeode->setStateSet( HUDStateSet );
	HUDStateSet->setTextureAttributeAndModes(0,HUDTexture,osg::StateAttribute::ON);

	// For this state set, turn blending on (so alpha texture looks right)
	HUDStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);

	// Disable depth testing so geometry is draw regardless of depth values
	// of geometry already draw.
	HUDStateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	HUDStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	HUDStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF );

	// Need to make sure this geometry is draw last. RenderBins are handled
	// in numerical order so set bin number to 11
	HUDStateSet->setRenderBinDetails( 11, "RenderBin");
}

Gui::~Gui()
{

}

void Gui::update(int xPos, int yPos)
{
	crosshairTransform->setMatrix(osg::Matrixd::translate(osg::Vec3f(xPos - _width/2, -(yPos - _height/2), 0.0f)));
}

int Gui::addGuiObject(int width, int height, int xPos, int yPos, std::string filename, float transparency)
{
	_guiObjects.push_back(new osg::Geode());
	_objectTransforms.push_back(new osg::MatrixTransform());

	_objectTransforms.back()->setMatrix(osg::Matrixd::translate(osg::Vec3f(xPos - _width/2, -(yPos - _height/2), 0.0f)));

	float cx = (_width - width) / 2;
	float cy = (_height - height) / 2;
	osg::Vec3Array* vertecies = new osg::Vec3Array();
	vertecies->push_back( osg::Vec3( cx     , cy     , -1) );
	vertecies->push_back( osg::Vec3( cx+width, cy     , -1) );
	vertecies->push_back( osg::Vec3( cx+width, cy+height, -1) );
	vertecies->push_back( osg::Vec3( cx     , cy+height, -1) );

	osg::DrawElementsUInt* indecies = new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0);
	indecies->push_back(0);
	indecies->push_back(1);
	indecies->push_back(2);
	indecies->push_back(3);

	osg::Vec4Array* colors = new osg::Vec4Array();
	colors->push_back(osg::Vec4(1,1,1,transparency));

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);

	osg::Geometry* geometry = new osg::Geometry();
	geometry->setTexCoordArray(0, texcoords);
	
	osg::Texture2D* texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	osg::Image* image;
	image = osgDB::readImageFile(filename.c_str());
	texture->setImage(image);

	geometry->addPrimitiveSet( indecies );
	geometry->setVertexArray( vertecies );
	geometry->setColorArray( colors );
	geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

	_guiObjects.back()->addDrawable(geometry);

	osg::StateSet* stateSet = new osg::StateSet();
	_guiObjects.back()->setStateSet(stateSet);
	stateSet->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
	stateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
	stateSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF );
	stateSet->setRenderBinDetails( 11, "RenderBin");

	_objectTransforms.back()->addChild(_guiObjects.back());
	_modelViewMatrix->addChild(_objectTransforms.back());

	return _guiObjects.size() - 1;
}

int Gui::addText(int fontSize, int xPos, int yPos, std::string text, std::string fontName)
{
	_textGeodes.push_back(new osg::Geode());
	_textObjects.push_back(new osgText::Text());

	_modelViewMatrix->addChild(_textGeodes.back());
	_textGeodes.back()->addDrawable(_textObjects.back());

	_textObjects.back()->setCharacterSize(fontSize);
	_textObjects.back()->setFont(fontName.c_str());
	_textObjects.back()->setText(text.c_str());
	_textObjects.back()->setAxisAlignment(osgText::Text::SCREEN);
	_textObjects.back()->setPosition( osg::Vec3(xPos, yPos, 0) );
	_textObjects.back()->setColor( osg::Vec4(199, 77, 15, 1) );

	return _textObjects.size() - 1;
}