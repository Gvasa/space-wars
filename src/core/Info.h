#ifndef INFO_H
#define INFO_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Info
{
  public:
    Info();
    ~Info();
    
    static int getXresolution() {return _xResolution;}
    static int getYresolution() {return _xResolution;}
    static void setXresolution(int x) {_xResolution = x;}
    static void setYresolution(int y) {_yResolution = y;}

    static float getPlayerSpeed() {return _playerSpeed;}
    static glm::vec3 getPlayerAngularVelocity() {return _playerAngularVelocity;}
    static glm::vec3 getPlayerLinearVelocity() {return _playerLinearVelocity;}
    static void setPlayerSpeed(float playerSpeed) {_playerSpeed = playerSpeed;}
    static void setPlayerAngularVelocity(glm::vec3 angularVelocity) {_playerAngularVelocity = angularVelocity;}
    static void setPlayerLinearVelocity(glm::vec3 linearVelocity) {_playerLinearVelocity = linearVelocity;}

  private:
    static int _xResolution;
    static int _yResolution;

    static float _playerSpeed;
    static glm::vec3 _playerAngularVelocity;
    static glm::vec3 _playerLinearVelocity; 
};

#endif