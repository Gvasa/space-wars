#ifndef GUI_H
#define GUI_H

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

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
  	Gui(int width = 1920, int height = 1080);
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
      \param fontSize The size of the font.
      \param xPos Position along the x-axis.
      \param yPos Position along the y-axis.
      \param fontName The name of the font including path.
      \param text The text of this element.
      \return Index of the added text
    */
    int addText(int fontSize, int xPos, int yPos, std::string text, std::string fontName);

    //! Change the text of the text object with index i.
    void changeText(int i, std::string text);

    /**
     * @brief Add a text GUI element from an obejct of type T.
     * @details Takes a pointer to an obejct of type T and a pointer to a function that converts that object to a std::string. Using these two pointers this object is then displayed as a GUI text element. As long as the pointer remains non-NULL the text will update automatically.
     * 
     * @param xPos Position along the x-axis. 
     * @param yPos Position along the y-axis
     * @param textObject Pointer to the object that is to be displayed.
     * @param toString Pointer to a function that converts an object of type T to a std::string.
     */
    template<typename T>
    void addText(int xPos, int yPos, T* textObject, std::string (*toString) (T* obj), int fontSize = 25, std::string fontName = "");
	
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