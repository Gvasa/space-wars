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
    */
    void addGuiObject(int width, int height, int xPos, int yPos, std::string filename, float transparency = 1.0f);
	
	private:
	 int currentRole;
   int _width;
   int _height;
   osg::MatrixTransform* crosshairTransform;

   std::vector<osg::Geode*> _guiObjects;
   std::vector<osg::MatrixTransform*> _objectTransforms;

};


#endif