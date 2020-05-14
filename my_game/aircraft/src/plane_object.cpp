#include "plane_object.h"



PlaneObject::PlaneObject() 
    :position(0, 0), size(1, 1), velocity(0.0f), sprite(), bullet_tex(),plane_gas(),color(1.0f), rotation(0.0f), IntervalTime(0),Role("enemy"),PlaneLife(1),Destroyed(false)
{

}

PlaneObject::PlaneObject(glm::vec2 pos, glm::vec2 sz, glm::vec2 vel, string role,texture2D sprite ,texture2D b_tex,texture2D gas,glm::vec3 clr )
    :position(pos), size(sz), velocity(vel),sprite(sprite), bullet_tex(b_tex),plane_gas(gas),color(clr), rotation(0.0f),IntervalTime(0),Role(role),PlaneLife(1),Destroyed(false)
{

}



void  PlaneObject::Reset(glm::vec2 pos, glm::vec2 vel)
{
	this->position = pos;
    this->velocity = vel;   
}


void  PlaneObject::Fire(GLfloat dt,GLuint window_width, GLuint window_height)
{

	this->IntervalTime += TIME_CONST *dt;
	if(this->Role == "host")
	{
		if(this->bullets.size() > 0)
		{
			for(vector<Bullet>::iterator it = this->bullets.begin();it != this->bullets.end(); )
			{
				it->Position -= it->Velocity * dt;
				//cout<<it->Position.y<<endl;
				if(it->Position.y <= 0)
				{
					this->bullets.erase(it);
				}else{
					++it;
				}
				
			}
		}
		if(this->IntervalTime >= 100)
		{
			
			this->IntervalTime = 0;

			glm::vec2 pos = glm::vec2(this->position.x + this->size.x/2 - L_SIZE.x/2 , this->position.y -L_SIZE.y );		
			this->bullets.push_back(Bullet(pos,L_SIZE,L_VELOCITY));
		}
	}
	else if(this->Role == "enemy")
	{
		if(this->bullets.size() > 0)
		{
			for(vector<Bullet>::iterator it = this->bullets.begin();it != this->bullets.end(); )
			{
				it->Position.x  += it->Velocity.x * dt;
				it->Position.y  += it->Velocity.y * dt;
				cout<<it->Position.x<<"<----->"<<it->Position.y<<endl;
				if(it->Position.y >= window_height-it->Siz.y)
				{
					cout<<"why"<<endl;
					this->bullets.erase(it);
				}else{
					++it;
				}
				
			}
		}
		if(this->IntervalTime >= 100)
		{
			
			this->IntervalTime = 0;

			glm::vec2 pos = glm::vec2(this->position.x + this->size.x/2 - C_SIZE.x/2 , this->position.y +this->size.y);		
			this->bullets.push_back(Bullet(pos,C_SIZE,C_VELOCITY));
		}
	}
	
}


void  PlaneObject::Draw(SpriteRenderer &renderer)
{
	if(this->Role == "host")
	{		
		renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);

		for(Bullet &b : this->bullets)
		{
			//cout<<b.Life<<endl;
			renderer.DrawSprite(this->bullet_tex, b.Position, b.Siz, b.Rotation, b.Color);
		}
	}
	else if(this->Role == "enemy")
	{
		renderer.DrawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
		int i = 0 ;
		if(this->bullets.size() > 0)
		{
			for(Bullet &b : this->bullets)
			{
				i++;
				//cout<<i<<endl;
				renderer.DrawSprite(this->bullet_tex, b.Position, b.Siz, b.Rotation, b.Color);
			}
		}
	}
	
}

void PlaneObject::Move(GLfloat dt,GLuint window_width, GLuint window_heigh)
{
	if(this->Role == "enemy")
	{
		this->position = this->velocity *dt;
		if(this->position.y >= window_heigh)
			this->PlaneLife = 0;
	}
}