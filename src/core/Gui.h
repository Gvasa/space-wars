#ifndef GUI_H
#define GUI_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>

class Gui : public osg::Group
{
	public:
    //! Initializes the GUI given the resolution of the active window.
  	Gui(int width, int height);
  	~Gui();

    //! Updates all GUI elements.
    void update(int xPos, int yPos);

    //! Add an image as a GUI object.
    /*!
      \param width Width of the image.
      \param height Height of the image.
      \param filename Name of the image including path.
      \param xPos The GUI objects position along the x-axis.
      \param yPos The GUI objects position along the y-axis.
      \param transparency Transparency of the GUI object.
      \return Index of the added GUI object.
    */
    int addGuiObject(int width, int height, int xPos, int yPos, std::string filename, float transparency = 1.0f);

    //! Add text to the GUI.
    /*!
      \param fontFize The size of the font.
      \param xPos Position along the x-axis.
      \param yPos Position along the y-axis.
      \param fontName The name of the font including path.
      \return Index of the added text
    */
    int addText(int fontSize, int xPos, int yPos, std::string text, std::string fontName);
	
	private:
	 int currentRole;
   int _width;
   int _height;
   osg::MatrixTransform* crosshairTransform;

   std::vector<osg::Geode*> _guiObjects;
   std::vector<osg::MatrixTransform*> _objectTransforms;

   std::vector<osg::Geode*> _textGeodes;
   std::vector<osgText::Text*> _textObjects;

   osg::MatrixTransform* _modelViewMatrix;

};


#endif