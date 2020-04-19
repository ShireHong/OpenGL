#include "ball_object.h"


BallObject::BallObject() 
    : GameObject(), Radius(12.5f), Stuck(true) 
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 vel, texture2D sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), vel), Radius(radius), Stuck(true) 
{

}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
	if(!this->Stuck)
	{
		this->position += velocity * dt;
		if(this->position.x <= 0)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = 0;
		}
		else if(this->position.x + this->size.x >= window_width)
		{
			this->velocity.x = -this->velocity.x;
			this->position.x = window_width - this->size.x;
		}
		if(this->position.y <= 0)
		{
			this->velocity.y = -this->velocity.y;
			this->position.y = 0;
		}
	}
	return this->position;
}

void  BallObject::Reset(glm::vec2 pos, glm::vec2 vel)
{
	this->position = pos;
    this->velocity = vel;
    this->Stuck = GL_TRUE;
    this->Sticky = GL_FALSE;
    this->PassThrough = GL_FALSE;
}