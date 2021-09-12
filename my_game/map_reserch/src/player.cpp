#include <algorithm>
#include <sstream>

#include "resource_manage.h"
#include "sprite_renderer.h"
//#include "particle.h"
#include "text_renderer.h"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "player.h"

#define ROLE_SPEED  100

enum Direction{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

Player::Player()
		:position(0, 0), size(1, 1), sprite(),color(1.0f),rotation(0.0f),state(PLAYER_STOP),velocity(ROLE_SPEED)
{
}
		
Player::Player(glm::vec2 pos, glm::vec2 sz, texture2D sprite, glm::vec3 clr)
		:position(pos), size(sz), sprite(sprite),color(clr),rotation(0.0f),state(PLAYER_STOP),velocity(ROLE_SPEED)
{
	this->targetpos.x = this->position.x;
	this->targetpos.y = this->position.y;
}

Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 vectors[]={
		glm::vec2(0.0f, 1.0f),	// 上
		glm::vec2(1.0f, 0.0f),	// 右
		glm::vec2(0.0f, -1.0f), // 下
		glm::vec2(-1.0f, 0.0f)	// 左
	};
	GLfloat max = 0;
	int direction = -1;
	for(int i = 0;i < 4; i++)
	{
		GLfloat dot_res = glm::dot(target,vectors[i]);
		if(dot_res > max)
		{
			max = dot_res;
			direction = i;
		}
	}
	return (Direction)direction;
}

		
/*更新位置和状态*/
//参考这个https://blog.csdn.net/qq_32609795/article/details/111188882

GLuint ccnt = 0;
void Player::Action(float dt)
{			
	glm::vec2 diff = this->targetpos - this->position;

	GLfloat movedist = this->velocity * dt;

	GLfloat distance = glm::length(diff);

	if(distance > 0.1f )
	{
		GLfloat f = movedist/distance;
		//cout<<"-------f------"<<f<<endl;
		this->position.x += f * diff.x;
		this->position.y += f * diff.y;
		cout<<"----x-----"<<this->position.x <<"----y---"<<this->position.y<<endl;	
		ccnt++;
		if(ccnt == 50)
		{
			ccnt = 0;
			this->walkindex ++;
			this->walkindex = this->walkindex % 6;
			this->state = PLAYER_WALK;
		}
	}
	else
	{
		this->state = PLAYER_STOP;
	}

}
//size需要调整，不然图片动起来会失真
void Player::Draw(SpriteRenderer &renderer)
{
	switch(this->state)
	{
		case PLAYER_STOP:
			this->sprite = ResourceManager::GetTexture(this->stop[0]);
    		renderer.DrawSprite(this->sprite, glm::vec2(this->position.x-ROLE_SIZE.x/2,this->position.y-ROLE_SIZE.y), this->size, this->rotation, this->color);
			break;
		case PLAYER_WALK:			
			this->sprite = ResourceManager::GetTexture(this->walkleft[this->walkindex]);
    		renderer.DrawSprite(this->sprite, glm::vec2(this->position.x-ROLE_SIZE.x/2,this->position.y-ROLE_SIZE.y), this->size, this->rotation, this->color);
			break;
	}
}


