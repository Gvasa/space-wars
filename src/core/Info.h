#ifndef INFO_H
#define INFO_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//! Contains information useful throughout the entire system. 

class Info
{
  public:
    Info();
    ~Info();
    
    //! Get the current resolution across the x-axis
    static int getXresolution() {return _xResolution;}
    //! Get the current resolution across the y-axis
    static int getYresolution() {return _yResolution;}
    //! Set the current resolution across the x-axis.
    /*!
        Used by main.
    */
    static void setXresolution(int x) {_xResolution = x;}
    //! Set the current resolution across the y-axis.
    /*!
        Used by main.
    */
    static void setYresolution(int y) {_yResolution = y;}

    //! Get the speed currently set by the player.
    static float getPlayerSpeed() {return _playerSpeed;}
    //! Get the angular velocity of the player.
    static glm::vec3 getPlayerAngularVelocity() {return _playerAngularVelocity;}
    //! Get the linear velocity of the player.
    static glm::vec3 getPlayerLinearVelocity() {return _playerLinearVelocity;}
    //! Set the current speed of the player.
    /*!
        Used by Physics.
    */
    static void setPlayerSpeed(float playerSpeed) {_playerSpeed = playerSpeed;}
    //! Set the current angular velocity of the player.
    /*!
        Used by Physics.
    */
    static void setPlayerAngularVelocity(glm::vec3 angularVelocity) {_playerAngularVelocity = angularVelocity;}
    //! Set the current linear velocity of the player.
    /*!
        Used by Physics.
    */
    static void setPlayerLinearVelocity(glm::vec3 linearVelocity) {_playerLinearVelocity = linearVelocity;}

    /**
     * @brief Set the player position given bu sgct.
     * @details Used by main.
     * 
     * @param playerPosition The player position given by sgct.
     */
    static void setPlayerPosition(glm::vec3 playerPosition) { 
        _playerPosition = playerPosition;}

    /**
     * @brief Get the position of the player.
     * 
     * @return The players position.
     */
    static glm::vec3 getPlayerPosition() {
        return _playerPosition;}
  private:
    static int _xResolution;
    static int _yResolution;

    static float _playerSpeed;
    static glm::vec3 _playerAngularVelocity;
    static glm::vec3 _playerLinearVelocity;
    static glm::vec3 _playerPosition; 
};

#endif