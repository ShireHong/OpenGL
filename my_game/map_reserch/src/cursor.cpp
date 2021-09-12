#include "cursor.h"

const float SENSITIVITY =  1.0f;

bool firstmouse = true;


Cursor::Cursor()
	:position(0, 0), size(1, 1), sprite(),lastx(0),lasty(0),color(1.0f),rotation(0.0f),MouseSensitivity(SENSITIVITY)
{

}

Cursor::Cursor(glm::vec2 pos, glm::vec2 sz, texture2D sprite, float firstx, float firsty, glm::vec3 clr) 
    : position(pos), size(sz), sprite(sprite), lastx(firstx),lasty(firsty),color(clr),rotation(0.0f),MouseSensitivity(SENSITIVITY)
 {
 	
 }
void Cursor::ArrowMove(float xpos, float ypos,unsigned int width_limit, unsigned int height_limit)
{
	if (firstmouse)
    {
        this->lastx = xpos;
        this->lasty = ypos;
        firstmouse = false;
    }

    float xoffset = xpos - this->lastx;
    float yoffset = ypos - this->lasty; // reversed since y-coordinates go from bottom to top

	xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

	this->position.x += xoffset;
	this->position.y += yoffset;
	if(this->position.x < 0)
	{
		this->position.x = 0;
	}
	else if(this->position.x > width_limit)
	{
		this->position.x = width_limit;
	}
	if(this->position.y < 0)
	{
		this->position.y = 0;
	}
	else if(this->position.y > height_limit)
	{
		this->position.y = height_limit;
	}
	this->lastx = xpos;
	this->lasty = ypos;
}

void Cursor::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}