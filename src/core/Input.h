#ifndef INPUT_H
#define INPUT_H

#include <iostream>

//! Handles all inputs from the player.
class Input
{

  public:
    //! Amount of avaliable commands.
    static const int NR_OF_COMMANDS = 8;

    //! List of all available commands.
    enum command {FORWARD = 0, BACKWARD = 1, LEFT = 2, RIGHT = 3, TILT_LEFT = 4, TILT_RIGHT = 5, FIRE = 6, RESET = 7};
    
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
    //! Checks the mouse input revived from sgct.
    /*!
        \param key The key that has been pressed.
        \param action Marks if the key has been pressed or held down.
    */
    void mouseCallback(const int& key, const int& action);
    //! Updates the mouse position. Not currently used.
    static void setMousePosition(const double& xPos, const double& yPos);
    
    //! Checks if the specified command is active or not.
    /*!
        \param cmd The command to check.
        \return The state of the given command.
    */
    bool getCommandState(enum command cmd);
    /*!
        \return The position of the mouse along the x-axis.
    */
    static double getMousePositionX();
    /*!
        \return The position of the mouse along the y-axis.
    */
    static double getMousePositionY();
    //! Gives the mouse position along the x- and y-axis using two pass-by-reference arguments.
    /*!
        \param xPos Stores the position of the mouse along the x-axis.
        \param yPos Stores the position of the mouse along the y-axis.
    */
    static void getMousePosition(double& xPos, double& yPos);

    /**
     * @brief Set the values of array to the current state of all commands.
     * @details array must be same length as Input::NR_OF_COMMADS
     * 
     * @param array the array to store the values in.
     */
    void getCommandsAsArray(bool* array);

    /**
     * @brief Update all commads with the values from array.
     * @details array must be same length as Input::NR_OF_COMMADS
     * 
     * @param array The array contaning the values.
     */
    void setCommadsFromArray(bool* array);

  private:
    static const int REPEAT = 2, PRESS = 1, MOUSE_LEFT = 0;
    bool _commandStates[NR_OF_COMMANDS];
    static double _xPosition, _yPosition;
};

#endif