#ifndef _CURSOR_H
#define _CURSOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


/*
定义游戏中的各元素的位置，大小，纹理，速度和颜色，包括所有的变量，并且可以调用spriterender渲染这些元素。
*/

class Cursor
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec3 color;
	GLfloat   rotation;
	GLfloat   lastx;
	GLfloat   lasty;
	float MouseSensitivity;
	
	texture2D sprite;

	Cursor();
	Cursor(glm::vec2 pos, glm::vec2 sz, texture2D sprite, float firstx, float firsty, glm::vec3 clr);

	virtual void Draw(SpriteRenderer &renderer);
	void ArrowMove(float xpos, float ypos,unsigned int width_limit, unsigned int height_limit);
};


#endif 