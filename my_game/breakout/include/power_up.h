#ifndef  _POWER_UP_H
#define  _POWER_UP_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"

using namespace std;

const glm::vec2 SIZE(60,20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	string 		Type;
	GLfloat 	Durations;
	GLboolean   Activated;

	PowerUp(string type, glm::vec3 color, GLfloat durations, glm::vec2 position, texture2D tex)
		:GameObject(position, SIZE, tex, color, VELOCITY),Type(type), Durations(durations),Activated()
		{
		}
};


#endif