#ifndef _PLAYER_H
#define _PLAYER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


/*
定义游戏中的各元素的位置，大小，纹理，速度和颜色，包括所有的变量，并且可以调用spriterender渲染这些元素。
*/
const glm::vec2 ROLE_SIZE(54, 110);


enum PlayerState{
	PLAYER_STOP,
	PLAYER_WALK,
	PLAYER_ATK,
};
class Player
{
public:	
	PlayerState state;
	glm::vec2 position;
	glm::vec2 size;
	glm::vec3 color;
	GLint     velocity;
	GLfloat   rotation;

	glm::vec2 targetpos;
	string stop[6];
	string walkleft[6];
	GLuint walkindex;
	texture2D sprite;

	Player();
		
	Player(glm::vec2 pos, glm::vec2 sz, texture2D sprite, glm::vec3 clr);

	void Action(float dt);	
	
	virtual void Draw(SpriteRenderer &renderer);
};


#endif 
