
#include "projectileFunctions.h"


void projectileFunctions::createLaser(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, glm::vec3 view, glm::vec3 pos, glm::mat4 ViewMat, double checktime){

	checktime = sgct::Engine::getTime();

	osg::Box* projectile = new osg::Box( osg::Vec3(0.0f, -0.3f, -250.0f), 0.1f, 0.1f, 500.0f);
	osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
	projectileDrawable->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 0.5f));
	projectileDrawable->computeBound();
	osg::Geode* projectileGeode = new osg::Geode();
	projectileGeode->addDrawable(projectileDrawable);

	osg::MatrixTransform* trans = new osg::MatrixTransform;

	trans->addChild(projectileGeode);

	glm::mat4 temp =  glm::translate( glm::mat4(1.0f), glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f))
						 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));
	mSceneTrans->addChild(trans);

}

void projectileFunctions::createProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, glm::vec3 view, glm::vec3 pos, glm::mat4 ViewMat, double checktime2[],
	Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays){

			osg::MatrixTransform* trans = new osg::MatrixTransform;


			if(laserpos % 2 != 0){

				osg::Box* projectile = new osg::Box( osg::Vec3(-0.5f, 0.0f, 0.0f), 0.1f, 0.1f, 0.4f);
				osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
				projectileDrawable->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 0.5f));
				projectileDrawable->computeBound();
				osg::Geode* projectileGeode = new osg::Geode();
				projectileGeode->addDrawable(projectileDrawable);
				trans->addChild(projectileGeode);

				projectilepos = glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f);

				glm::mat4 temp =  glm::translate( glm::mat4(1.0f), projectilepos)
								 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				laserpos++;

				trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));

			}
			else{

				osg::Box* projectile = new osg::Box( osg::Vec3(0.5f, 0.0f, 0.0f), 0.1f, 0.1f, 0.4f);
				osg::ShapeDrawable* projectileDrawable = new osg::ShapeDrawable(projectile);
				projectileDrawable->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 0.5f));
				projectileDrawable->computeBound();
				osg::Geode* projectileGeode = new osg::Geode();
				projectileGeode->addDrawable(projectileDrawable);
				trans->addChild(projectileGeode);
				

				projectilepos = glm::vec3(-view.x-pos.x, -view.y-pos.y, -view.z-pos.z+10.0f);

				glm::mat4 temp =  glm::translate( glm::mat4(1.0f), projectilepos)
								 * glm::inverse(ViewMat) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				laserpos++;

				trans->setMatrix(osg::Matrixd(glm::value_ptr(temp)));
			}

			mSceneTrans->addChild(trans);

			viewdir = view;

			ViewArray = ViewMat;

			positions.add(projectilepos);
			viewdirections.add(viewdir);
			viewarrays.add(ViewArray);

			checktime2[i] = sgct::Engine::getTime();
			i++;
			if(i == size)
				i = 0;
}

void projectileFunctions::moveProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, sgct::Engine * gEngine, Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays){

		Set<glm::vec3>::Node* temp1 = positions.head->next;
		Set<glm::vec3>::Node* temp2 = viewdirections.head->next;
		Set<glm::mat4>::Node* temp3 = viewarrays.head->next;

		for(int q = 202; q < mSceneTrans->getNumChildren(); q++){

				glm::mat4 tempen = glm::translate( glm::mat4(1.0f), temp1->pos)
								 		* glm::inverse(temp3->pos) * glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

				mSceneTrans->getChild(q)->asTransform()->asMatrixTransform()->setMatrix(osg::Matrixd(glm::value_ptr(tempen)) );

				temp1->pos += (-temp2->pos * projectile_speed * static_cast<float>(gEngine->getDt()) );

				temp1 = temp1->next;
				temp2 = temp2->next;
				temp3 = temp3->next;

		}
}

void projectileFunctions::removeProjectile(osg::ref_ptr<osg::MatrixTransform> mSceneTrans, double checktime2[], Set<glm::vec3> &positions, Set<glm::vec3> &viewdirections, Set<glm::mat4> &viewarrays){

	if(checktime2[j] != 0.0 && checktime2[j]+10.0 < sgct::Engine::getTime()){
			mSceneTrans->removeChild(numchild, 1);

			checktime2[j] = 0.0;

			positions.remove();
			viewdirections.remove();
			viewarrays.remove();

			j++;
			if(j == size)
				j = 0;

	}

}