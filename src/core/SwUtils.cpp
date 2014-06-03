#include "SwUtils.h"

glm::mat4 swutils::bulletTransToGlmMat4(btTransform b)
{
    btScalar a[16];
    b.getOpenGLMatrix(a);
    glm::mat4 temp;
    temp[0][0] = a[0];
    temp[0][1] = a[1];
    temp[0][2] = a[2];
    temp[0][3] = a[3];

    temp[1][0] = a[4];
    temp[1][1] = a[5];
    temp[1][2] = a[6];
    temp[1][3] = a[7];

    temp[2][0] = a[8];
    temp[2][1] = a[9];
    temp[2][2] = a[10];
    temp[2][3] = a[11];

    temp[3][0] = a[12];
    temp[3][1] = a[13];
    temp[3][2] = a[14];
    temp[3][3] = a[15];

    return temp;
}

glm::mat4 swutils::bulletMat3ToGlmMat4(btMatrix3x3 m)
{
    glm::mat4 temp;
    temp[0][0] = m[0][0];
    temp[0][1] = m[0][1];
    temp[0][2] = m[0][2];
    temp[0][3] = 0;

    temp[1][0] = m[1][0];
    temp[1][1] = m[1][1];
    temp[1][2] = m[1][2];
    temp[1][3] = 0;

    temp[2][0] = m[2][0];
    temp[2][1] = m[2][1];
    temp[2][2] = m[2][2];
    temp[2][3] = 0;

    temp[3][0] = 0;
    temp[3][1] = 0;
    temp[3][2] = 0;
    temp[3][3] = 1;

    return temp;
}

btVector3 swutils::glmVec3ToBulletVec3(glm::vec3 v)
{
    return btVector3(v.x,v.y,v.z);
}

std::string swutils::glmVec2PtrToString(glm::vec2* v)
{
    std::string str;
    str += v->x;
    str += " ";
    str += v->y;

    return str;
}

btTransform swutils::glmMat4ToBullletTranform(glm::mat4 m)
{   
    btScalar a[16];
    a[0] = m[0][0];
    a[1] = m[0][1];
    a[2] = m[0][2];
    a[3] = m[0][3];

    a[4] = m[1][0];
    a[5] = m[1][1];
    a[6] = m[1][2];
    a[7] = m[1][3];

    a[8] = m[2][0];
    a[9] = m[2][1];
    a[10] = m[2][2];
    a[11] = m[2][3];

    a[12] = m[3][0];
    a[13] = m[3][1];
    a[14] = m[3][2];
    a[15] = m[3][3];

    btTransform b;
    b.setFromOpenGLMatrix(a);
    return b;
}

void swutils::printMat4(glm::mat4 m)
{
    std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl <<
              m[1][0]  << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl <<
              m[2][0]  << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl <<
              m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl; 

    std::cout << "-----------------------" << std::endl;
}