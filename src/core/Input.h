#ifndef INPUT_H
#define INPUT_H

#include <iostream>

//! Handles all inputs from the player.
class Input
{

  public:
    //! List of all available commands.
    enum command {FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3, TILT_LEFT = 4, TILT_RIGHT = 5};
    
    //! Basic constructor. 
    Input();
    //! Empty destructor.
    ~Input();

    //! Checks the input revived from sgct.
    /*!
        \param key The key that has been pressed.
        \param action Marks if the key has been pressed or held down.
    */
    void keyCallback(const int& key, const int& action);
    //! Updates the mouse position. Not currently used.
    void setMousePosition(const double& xPos, const double& yPos);
    
    //! Checks if the specified command is active or not.
    /*!
        \param cmd The command to check.
        \return The state of the given command.
    */
    bool getCommandState(enum command cmd);
    /*!
        \return The position of the mouse along the x-axis.
    */
    double getMousePositionX();
    /*!
        \return The position of the mouse along the y-axis.
    */
    double getMousePositionY();
    //! Gives the mouse position along the x- and y-axis using two pass-by-reference arguments.
    /*!
        \param xPos Stores the position of the mouse along the x-axis.
        \param yPos Stores the position of the mouse along the y-axis.
    */
    void getMousePosition(double& xPos, double& yPos);

  private:
    static const int NR_OF_COMMANDS = 6;
    static const int REPEAT = 2, PRESS = 1;
    bool _commandStates[NR_OF_COMMANDS];
    double _xPosition = 0, _yPosition = 0;
};

#endif