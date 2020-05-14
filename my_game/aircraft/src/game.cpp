#include <algorithm>
#include <sstream>

#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
#include "particle.h"
#include "plane_object.h"

#include <iostream>
#include <stdlib.h>


SpriteRenderer  	*Renderer;
PlaneObject         *HostPlane;
PlaneObject			*EnemyPlane;
ParticleGenerator 	*Particles;

Game::Game(unsigned int w, unsigned int h)
		: State(GAME_MENU), Keys(), Width(w), Height(h)
{

}

Game::~Game()
{
	delete Renderer;
	delete HostPlane;
	delete EnemyPlane;
}

void Game::Init()
{
	
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	ResourceManager::LoadShader("resource/shader/particle.vs", "resource/shader/particle.fs", nullptr, "particle" );
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 	0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);

    // Load textures
    ResourceManager::LoadTexture("resource/texture/bk5.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("resource/texture/plane3.png", GL_TRUE, "p1");
     ResourceManager::LoadTexture("resource/texture/enemy1.png", GL_TRUE, "e1");
    ResourceManager::LoadTexture("resource/texture/gas1.png", GL_TRUE, "gas1");
    ResourceManager::LoadTexture("resource/texture/bullet1.png", GL_TRUE, "bullet1");
    ResourceManager::LoadTexture("resource/texture/bullet2.png", GL_TRUE, "bullet2");
    #if 1
    ResourceManager::LoadTexture("resource/texture/attack1.png", GL_TRUE, "at1");
    ResourceManager::LoadTexture("resource/texture/attack2.png", GL_TRUE, "at2");
    ResourceManager::LoadTexture("resource/texture/attack3.png", GL_TRUE, "at3");
    #endif
    // Set render-specific controls


    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    glm::vec2 planepos = glm::vec2(this->Width / 2 - PLANE_SIZE.x / 2,  this->Height - PLANE_SIZE.y);

    HostPlane = new PlaneObject(planepos, PLANE_SIZE, PLANE_VELOCITY,"host",ResourceManager::GetTexture("p1"), ResourceManager::GetTexture("bullet1"),ResourceManager::GetTexture("gas1"));

   #if 0
    glm::vec2 pos = glm::vec2(300,-PLANE_SIZE.y);
    EnemyPlane = new PlaneObject(pos, PLANE_SIZE, ENEMY_VELOCITY,"enemy",ResourceManager::GetTexture("e1"), ResourceManager::GetTexture("bullet2"),ResourceManager::GetTexture("gas1"));
    #endif
}

void Game::ProcessInput(float dt)
{
	glm::vec2 vel = PLANE_VELOCITY *dt;
	if(this->Keys[GLFW_KEY_A] == GL_TRUE )
	{
		if(HostPlane->position.x >= 0)
		{
			HostPlane->position.x -= vel.x;			
		}
	}
	if(this->Keys[GLFW_KEY_D] == GL_TRUE)
	{
		if(HostPlane->position.x <= this->Width - HostPlane->size.x)
		{
			HostPlane->position.x += vel.x;			
		}
	}
	if(this->Keys[GLFW_KEY_W] == GL_TRUE)
	{
		if(HostPlane->position.y >= 0)
		{
			HostPlane->position.y -= vel.y;
		}
	}
	if(this->Keys[GLFW_KEY_S] == GL_TRUE)
	{
		if(HostPlane->position.y <= this->Height - HostPlane->size.y)
		{
			HostPlane->position.y += vel.y;
		}
	} 	
}

GLfloat enemy_time = 0;

void Game::EnemyUpdate(float dt)
{	
	if(this->enemys.size() > 0)
	{
		for(vector<PlaneObject>::iterator it = this->enemys.begin();it != this->enemys.end(); )
		{
			it->position += it->velocity * dt;
			//cout<<it->Position.y<<endl;

			if(it->position.y >= this->Height || it->Destroyed)
			{
				this->enemys.erase(it);
			}else{
				++it;
			}
			
		}
	}

	enemy_time += 200*dt;
	if(enemy_time >= 100)
	{
		enemy_time = 0;
		GLuint rate = rand()%480;
		glm::vec2 pos = glm::vec2(rate,-PLANE_SIZE.y);
		this->enemys.push_back(PlaneObject(pos, PLANE_SIZE, ENEMY_VELOCITY,"enemy",ResourceManager::GetTexture("e1"), ResourceManager::GetTexture("bullet2"),ResourceManager::GetTexture("gas1")));

	}
}


void Game::UpDate(float dt)
{
	int i=0;
	this->Map_y  += dt * MAP_VELOCITY;
	if(this->Map_y > 800) this->Map_y=0;

	this->DoCollisions();

	HostPlane->Fire(dt,this->Width,this->Height);
	//EnemyPlane->Move(dt,this->Width,this->Height);
	

	#if 1
	this->EnemyUpdate(dt);
	for(PlaneObject &p: this->enemys)
	{
		i++;
		
		
		p.Fire(dt,this->Width,this->Height);
	}
	#else
	EnemyPlane->Fire(dt,this->Width,this->Height);
	#endif
}

static GLfloat x=0;
static GLuint s = 1;

void Game::Render(float dt)
{
	x++;
	if(x > 450) x=0;
	
	//cout<<y<<endl;
	Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, this->Map_y), glm::vec2(this->Width, this->Height ), 0.0f);
	Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0,-800+this->Map_y), glm::vec2(this->Width, this->Height ), 0.0f);	

	HostPlane->Draw(*Renderer);
	
	#if 1
	for(PlaneObject &p: this->enemys)
	{
		if(!p.Destroyed)		
			p.Draw(*Renderer);
	}
	#else
	EnemyPlane->Draw(*Renderer);
	#endif
	#if 1
	if(s == 1 && x <= 150)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("at1"), glm::vec2(this->Width/2,400), glm::vec2(78, 58 ), 0.0f);
		if(x == 150)
			s=2;
		cout<<s<<x<<endl;
	}
	else if(s == 2 && x>150 && x <= 300)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("at2"), glm::vec2(this->Width/2,400), glm::vec2(78, 58 ), 0.0f);
		if(x == 300)
			s=3;
		cout<<s<<x<<endl;
	}
	else if(s == 3 && x > 300 && x <= 450)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("at3"), glm::vec2(this->Width/2,400), glm::vec2(78, 58 ), 0.0f);
		if(x == 450)
		  	s=1;
		cout<<s<<x<<endl;
	}
	#endif
	
}

GLboolean CheckCollision(Bullet &one, PlaneObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    GLboolean collisionX = one.Position.x + one.Siz.x >= two.position.x &&
        two.position.x + two.size.x >= one.Position.x;
    // Collision y-axis?
    GLboolean collisionY = one.Position.y + one.Siz.y >= two.position.y &&
        two.position.y + two.size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

void Game::DoCollisions()
{
	for(Bullet &b: HostPlane->bullets)
	{
		for(PlaneObject &p:this->enemys)
		{
			if(!p.Destroyed )
			{
				if(CheckCollision(b, p))
				{
					p.Destroyed = GL_TRUE;
				}
			}
		}
	}
}