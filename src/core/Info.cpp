#include "Info.h"

int Info::_xResolution = 0;
int Info::_yResolution = 0;

float Info::_playerSpeed = 0;
glm::vec3 Info::_playerAngularVelocity(0.0f, 0.0f, 0.0f);
glm::vec3 Info::_playerLinearVelocity(0.0f, 0.0f, 0.0f); 