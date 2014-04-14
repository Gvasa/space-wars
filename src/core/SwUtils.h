#ifndef SWUTILS_H
#define SWUTILS_H

#include <btBulletDynamicsCommon.h>
#include <osg/MatrixTransform>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace swutils
{
    glm::mat4 bulletTransToGlmMat4(btTransform b);
    glm::mat4 bulletMat3ToGlmMat4(btMatrix3x3 m);
    btVector3 glmVec3ToBulletVec3(glm::vec3 v);
}



#endif