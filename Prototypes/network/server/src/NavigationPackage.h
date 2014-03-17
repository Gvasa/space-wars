#ifndef NAVIGATIONPACKAGE_H
#define NAVIGATIONPACKAGE_H

#include <glm/glm.hpp>

class NavigationPackage
{
  public:
    NavigationPackage()
    {
      position = glm::vec3(0,0,0);
      verticalAngle = 0;
      horizontalAngle = 0;
    }

    NavigationPackage(glm::vec3 pos, float vert, float hor)
    {
      position = pos;
      verticalAngle = vert;
      horizontalAngle = hor;
    }
    
    glm::vec3 position;
    float verticalAngle;
    float horizontalAngle;
};

#endif