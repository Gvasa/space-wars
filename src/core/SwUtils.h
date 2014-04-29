#ifndef SWUTILS_H
#define SWUTILS_H

#include <string>

#include <btBulletDynamicsCommon.h>
#include <osg/MatrixTransform>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//! Space-wars utility library.
/*!
    Contains utility functions to convert data types between space-wars different dependencies. 
*/
namespace swutils
{   
    //! Converts a bullet btTransform to a glm::mat4.
    /*!
        \param b btTransform that is to be converted. 
        \return The resulting glm::mat4.
    */
    glm::mat4 bulletTransToGlmMat4(btTransform b);

    //! Converts a bullet btMatrix3x3 to a glm::mat4.
    /*!
        Converts and ads homogeneous coordinates.
        \param m btMatirx3x3 that is to be converted.
        \return The resulting glm::mat4.
    */
    glm::mat4 bulletMat3ToGlmMat4(btMatrix3x3 m);

    //! Converts a glm::vec3 to a btVector3.
    /*!
        \param v glm::vec3 that is to be converted.
        \return The resulting btVector3.
    */
    btVector3 glmVec3ToBulletVec3(glm::vec3 v);

    /**
     * @brief Converts a glm:vec2 to std::string.
     * @details Converts the glm::vec2 pointed to by v to std::string. 
     * 
     * @param v Pointer to the object to be converted.
     * @return The string representing the glm::vec2 pointed to by v.
     */
    std::string glmVec2PtrToString(glm::vec2* v);

}

#endif