#ifndef PROJECTILEFUNCTIONS_H
#define PROJECTILEFUNCTIONS_H

#include "sgct.h"
#include <iostream>
#include <string>
#include "enet/enet.h"
#include <queue>
 
#include <thread>
#include <mutex>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <btBulletDynamicsCommon.h>
#include <osg/TextureCubeMap>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
 
#include "skybox.h"
#include "Package.h"
#include "NavigationPackage.h"
#include "set.cpp"


class projectileFunctions
{
private:

glm::vec3 viewdir;
glm::vec3 projectilepos;
glm::mat4 ViewArray;

const int size = 1000;

int checkchild;
int laserpos = 1;

float projectile_speed = 50.0f;

int numchild = 202;
int i = 0;
int j = 0;


public:

	void createLaser(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, glm::vec3 view, glm::vec3 pos, glm::mat4 ViewMat, double checktime);

	void createProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, glm::vec3 view, glm::vec3 pos, glm::mat4 ViewMat, double checktime2[],
	Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays);


	void moveProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, sgct::Engine * gEngine, Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays);

	void removeProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, double checktime2[], Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays);

};

#endif