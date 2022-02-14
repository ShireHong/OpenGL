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


Player::Player()
		:position(0, 0), size(1, 1), sprite(),color(1.0f),rotation(0.0f),state(PLAYER_STOP),velocity(ROLE_SPEED)
{
}
		
Player::Player(glm::vec2 pos, glm::vec2 sz, texture2D sprite, glm::vec3 clr)
		:position(pos), size(sz), sprite(sprite),color(clr),rotation(0.0f),state(PLAYER_STOP),velocity(ROLE_SPEED)
{
	this->targetpos.x = this->position.x;
	this->targetpos.y = this->position.y;
	walkindex = 0;
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

char path[200];
void Player::Action(float dt)/*mapinfo*/
{	
	GLint pathlen;
	PathNode node;
	
	/*如果当前坐标和目标坐标不相等且xyset为空*/
	//cout<<position.x<<","<<position.y<<endl;
	//cout<<"target: "<<targetpos.x<<","<<targetpos.y<<endl;
	if(state == PLAYER_STOP)
	{
		if((position.x != targetpos.x || position.y != targetpos.y) && xyset.empty())
		{
			//cout<<"target: "<<targetpos.x<<","<<targetpos.y<<endl;		
			pathlen = astarfindpath(position.x, position.y, targetpos.x, targetpos.y, 168, 120, path,xyset);
			//cout<<"here 1"<<endl;
			/*print path*/
			for(int i = 0; i < xyset.size();i++)
			{
				//cout<<xyset[i].map_x<<" "<<xyset[i].map_y<<" "<<xyset[i].dir<<endl;
			}
			cout<<endl;
		}
		/*开始导入第一个*/
		if(!xyset.empty())
		{
			cout<<position.x<<","<<position.y<<endl;
			//xyset.pop_back();
			node = xyset.back();
			targetpos.x = node.map_x;
			targetpos.y = node.map_y;
			dir 		= node.dir;
			xyset.pop_back();
			//cout<<"first: "<<targetpos.x<<","<<targetpos.y<<endl;			
		}
	}

	/*若路径vector不为空，且已经到达下一个路径，则更新目标坐标*/
	if(!xyset.empty())
	{
		if((position.x == targetpos.x && (dir == RIGHT || dir == LEFT) )
			|| (position.y == targetpos.y && (dir == DOWN || dir == UP)))
		{
			node = xyset.back();
			targetpos.x = node.map_x;
			targetpos.y = node.map_y;
			dir         = node.dir;
			xyset.pop_back();
			//cout<<"here 3"<<endl;
		}
	}

	/*若当前坐标不等于目标坐标，则继续按此方向运行*/
	if(position.x != targetpos.x || position.y != targetpos.y)
	{
		switch(dir)
		{	
			case RIGHT:
				position.x += velocity *dt;
				break;
			case DOWN:
				position.y += velocity *dt;
				break;
			case LEFT:
				position.x -= velocity *dt;
				break;
			case UP:
				position.y -= velocity *dt;
				break;
			defualt:
			    return;
		}
		state = PLAYER_WALK;
		position.x = (int)position.x;
		position.y = (int)position.y;
		//cout<<"target: "<<targetpos.x<<","<<targetpos.y<<endl;
		//cout<<position.x<<","<<position.y<<endl;
	}
	/*to do 
	 *这里需要判断x y都相等才能将状态设置为stop，这里先验证。
	 *
	 */
	else if(position.x == targetpos.x || position.y == targetpos.y)
	{
		if(xyset.empty())
		{
			//cout<<"stop"<<endl;
			state = PLAYER_STOP;
		}
	}

}
//size需要调整，不然图片动起来会失真
void Player::Draw(SpriteRenderer &renderer)
{
	switch(this->state)
	{
		case PLAYER_STOP:
			this->sprite = ResourceManager::GetTexture("down_0");
    		renderer.DrawSprite(this->sprite, glm::vec2(this->position.x-ROLE_SIZE.x/2,this->position.y-ROLE_SIZE.y), this->size, this->rotation, this->color);
			break;
		case PLAYER_WALK:
			#if 1
			cout<<dir<<" "<<walkindex<<endl;
			cout<<walk[dir][walkindex]<<endl;
			this->sprite = ResourceManager::GetTexture(walk[dir][walkindex]);
    		renderer.DrawSprite(this->sprite, 
    							glm::vec2(this->position.x-ROLE_SIZE.x/2,this->position.y-ROLE_SIZE.y), 
    							this->size, 
    							this->rotation, 
    							this->color);
		    walkindex++;
			walkindex = walkindex % 4;
			#endif
			break;
	}
}


