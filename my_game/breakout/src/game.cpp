#include <algorithm>
#include <sstream>

#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "game_level.h"
#include "particle.h"
#include "post_processor.h"
#include "text_renderer.h"

#include <iostream>
#include <stdlib.h>


SpriteRenderer  	*Renderer;
GameObject      	*Player;
BallObject      	*Ball;
ParticleGenerator 	*Particles;
PostProcessor       *Effects;
GLfloat				ShakeTime = 0.0f;
TextRenderer        *Text;

Game::Game(unsigned int w, unsigned int h)
		: state(GAME_MENU), keys(), width(w), height(h),Level(0), Lives(3) 
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
}

void Game::init()
{
	
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	ResourceManager::LoadShader("resource/shader/particle.vs", "resource/shader/particle.fs", nullptr, "particle" );
	ResourceManager::LoadShader("resource/shader/postprocessor.vs", "resource/shader/postprocessor.fs", nullptr, "postprocessing" );

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 
		0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);
    ResourceManager::GetShader("particle").use().set_int("image", 0);
    ResourceManager::GetShader("particle").set_mat4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("resource/texture/background.png", GL_FALSE, "background");
    ResourceManager::LoadTexture("resource/texture/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("resource/texture/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("resource/texture/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("resource/texture/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("resource/texture/particle.png", GL_TRUE, "particle");
    //道具
    ResourceManager::LoadTexture("resource/texture/powerup_speed.png", GL_TRUE, "powerup_speed");
    ResourceManager::LoadTexture("resource/texture/powerup_sticky.png", GL_TRUE, "powerup_sticky");
    ResourceManager::LoadTexture("resource/texture/powerup_increase.png", GL_TRUE, "powerup_increase");
    ResourceManager::LoadTexture("resource/texture/powerup_confuse.png", GL_TRUE, "powerup_confuse");
    ResourceManager::LoadTexture("resource/texture/powerup_chaos.png", GL_TRUE, "powerup_chaos");
    ResourceManager::LoadTexture("resource/texture/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");

    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    GameLevel one,two,three,four;

    one.Load("resource/level/one.lvl", this->width, this->height*0.5);
    two.Load("resource/level/two.lvl", this->width, this->height*0.5);
    three.Load("resource/level/three.lvl", this->width, this->height*0.5);
    four.Load("resource/level/four.lvl", this->width, this->height*0.5);

    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);

    this->Level = 0;
    //挡板
    glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    //球
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("face"));

    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->width, this->height);

    Text = new TextRenderer(this->width, this->height);
    Text->Load("resource/fonts/arial.ttf", 24);
}

void Game::process_input(float dt)
{
	if(this->state == GAME_ACTIVE)
	{
		GLfloat vel = PLAYER_VELOCITY *dt;
		if(this->keys[GLFW_KEY_A] == GL_TRUE )
		{
			if(Player->position.x >= 0)
			{
				Player->position.x -= vel;
				if(Ball->Stuck)
					Ball->position.x -= vel ;
			}
		}
		if(this->keys[GLFW_KEY_D] == GL_TRUE)
		{
			if(Player->position.x <= this->width - Player->size.x)
			{
				Player->position.x += vel;
				if(Ball->Stuck)
					Ball->position.x += vel ;
			}
		}
	  	if (this->keys[GLFW_KEY_SPACE])
        	Ball->Stuck = GL_FALSE;
	}
	if(this->state == GAME_MENU)
	{
		if(this->keys[GLFW_KEY_ENTER] == GL_TRUE && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->state = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if(this->keys[GLFW_KEY_W] == GL_TRUE && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1)%4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
		}
		if (this->keys[GLFW_KEY_S]== GL_TRUE && !this->KeysProcessed[GLFW_KEY_S])
	    {
	        if (this->Level > 0)
	            --this->Level;
	        else
	            this->Level = 3;
	        this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
	    }
	}
	if (this->state == GAME_WIN)
    {
        if (this->keys[GLFW_KEY_ENTER])
        {
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
            Effects->Chaos = GL_FALSE;
            this->state = GAME_MENU;
        }
    }
}

void Game::update(float dt)
{
	Ball->Move(dt,this->width);

	this->DoCollisions();

	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	this->UpdatePowerUps(dt);

	if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = GL_FALSE;
    }

	if (Ball->position.y >= this->height) // Did ball reach bottom edge?
    {
    	--this->Lives;
    	if(!this->Lives)
    	{
        	this->ResetLevel();
       		this->state = GAME_MENU;
    	}
    	this->ResetPlayer();
    }
    if (this->state == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
    {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = GL_TRUE;
        this->state = GAME_WIN;
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)
    	this->Levels[0].Load("resource/level/one.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("resource/level/two.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("resource/level/three.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("resource/level/four.lvl", this->width, this->height * 0.5f);
    this->Lives = 3;
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->size = PLAYER_SIZE;
    Player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Ball->Reset(Player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    Effects->Chaos = Effects->Confuse = GL_FALSE;
    Ball->PassThrough = Ball->Sticky = GL_FALSE;
    Player->color = glm::vec3(1.0f);
    Ball->color = glm::vec3(1.0f);
}


void Game::render()
{
	/*Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	*/
	if(this->state == GAME_ACTIVE || this->state == GAME_MENU)
	{

		Effects->BeginRender();
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);

        for(PowerUp &powerup : this->PowerUps)
        {
        	if(!powerup.destroyed)
        		powerup.Draw(*Renderer);
        }

        if(!Ball->Stuck)
        	Particles->Draw();

        Ball->Draw(*Renderer);

        Effects->EndRender();
        Effects->Render(glfwGetTime());

        std::stringstream ss;
        ss<<this->Lives;
        Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->state == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, this->height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, this->height / 2 + 20.0f, 0.75f);
    }
    if (this->state == GAME_WIN)
    {
        Text->RenderText(
            "You WON!!!", 320.0, this->height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
            "Press ENTER to retry or ESC to quit", 130.0, this->height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
}

Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 vectors[]={
		glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
	};
	GLfloat max = 0;
	int direction = -1;
	for(int i = 0;i < 4; i++)
	{
		GLfloat dot_res = glm::dot(target,vectors[i]);
		//cout<<dot_res<<endl;
		if(dot_res > max)
		{
			max = dot_res;
			direction = i;
		}
	}
	return (Direction)direction;
}


GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    GLboolean collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // Collision y-axis?
    GLboolean collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}


Collision CheckCollision(BallObject &one, GameObject &two) // AABB - AABB collision
{
#if 0
    // x轴方向碰撞？
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // y轴方向碰撞？
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // 只有两个轴向都有碰撞时才碰撞
    return collisionX && collisionY;
#endif
// 获取圆的中心 
    glm::vec2 center(one.position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.size.x / 2, two.size.y / 2);
    glm::vec2 aabb_center(
        two.position.x + aabb_half_extents.x, 
        two.position.y + aabb_half_extents.y
    );
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    //return glm::length(difference) < one.Radius;
    if(glm::length(difference) <= one.Radius)
    	return make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
    	return make_tuple(GL_FALSE, UP,glm::vec2(0,0));
}  

static void ActivatePowerUp(PowerUp &powerup);

void Game::DoCollisions()
{
	//球和目标的碰撞
	for(GameObject &box : this->Levels[this->Level].Bricks)
	{
		if(!box.destroyed) 
		{			
			Collision  coll_res = CheckCollision(*Ball,box);
			if(get<0>(coll_res))
			{
				if(!box.issolid)
				{
					box.destroyed = GL_TRUE;
					this->SpawnPowerUps(box);
				}
				else{
					ShakeTime = 0.05f;
            		Effects->Shake = GL_TRUE;
				}

				Direction dir  = get<1>(coll_res);
				cout<<"dir = "<<dir<<endl;
				glm::vec2 diff = get<2>(coll_res);
				cout<<"diff.x = "<<diff.x<<endl;
				cout<<"diff.y = "<<diff.y<<endl;
				if(!(Ball->PassThrough && !box.issolid))
				{
					if(dir == RIGHT || dir == LEFT)
					{
						Ball->velocity.x = -Ball->velocity.x;
						 GLfloat penetration = Ball->Radius - std::abs(diff.x);
	                    if (dir == LEFT)
	                        Ball->position.x -= penetration; // 将球右移
	                    else
	                        Ball->position.x += penetration; // 将球左移
	                }
	                else // 垂直方向碰撞
	                {
	                    Ball->velocity.y = -Ball->velocity.y; // 反转垂直速度
	                    // 重定位
	                    GLfloat penetration = Ball->Radius - std::abs(diff.y);
	                    if (dir == UP)
	                        Ball->position.y -= penetration; // 将球上移
	                    else
	                        Ball->position.y += penetration; // 将球下移
	                }	
				}
							
			}

		}
	}
	//检查挡板和球的碰撞情况
	Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->position.x + Player->size.x / 2;
        GLfloat distance = (Ball->position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
		/*球的速度会根据碰撞来变化，横坐标是根据打在挡板的位置，纵坐标是取反*/
        glm::vec2 oldVelocity = Ball->velocity;
        Ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        Ball->velocity.y = -Ball->velocity.y;
        Ball->velocity = glm::normalize(Ball->velocity) * glm::length(oldVelocity);
        //Ball->velocity.y = -1 * abs(Ball->velocity.y);
        Ball->Stuck = Ball->Sticky;
    } 
    //检查道具和挡板的碰撞情况
    for(PowerUp &powerup : PowerUps)
    {
    	if(!powerup.destroyed)
    	{
    		if(powerup.position.y >= this->height)
    		{
    			powerup.destroyed = GL_TRUE;
    		}
    		if (CheckCollision(*Player, powerup))
            {	// Collided with player, now activate powerup
                ActivatePowerUp(powerup);
                powerup.destroyed = GL_TRUE;
                powerup.Activated = GL_TRUE;
            }
    	}
    }

}

static GLboolean ShouldSpawn(GLuint chance)
{
	GLuint rate = rand() % chance;
	return rate==0;
}

void Game::SpawnPowerUps(GameObject &block)
{
	//一个块可以生成多个道具？
	if(ShouldSpawn(75))
	{
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::GetTexture("powerup_speed")));
	}
	if(ShouldSpawn(75))
	{
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::GetTexture("powerup_sticky")));
	}
	if(ShouldSpawn(75))
	{
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::GetTexture("powerup_passthrough")));
	}
	if(ShouldSpawn(75))
	{
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.position, ResourceManager::GetTexture("powerup_increase")));
	}
	if(ShouldSpawn(15))
	{
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::GetTexture("powerup_confuse")));
	}
	if(ShouldSpawn(15))
	{
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::GetTexture("powerup_chaos")));
	}
}

static void ActivatePowerUp(PowerUp &powerup)
{
	if(powerup.Type == "speed")
	{
		Ball->velocity *= 1.2;
	}
	else if(powerup.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->color = glm::vec3(1.0f, 0.5f, 1.0f); 
	}
	else if(powerup.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if(powerup.Type == "pad-size-increase")
	{
		Player->size.x += 50;
	}
	else if(powerup.Type == "confuse")
	{
		if(!Effects->Chaos)
			Effects->Confuse = GL_TRUE;
	}
	else if(powerup.Type == "chaos")
	{
		if(!Effects->Confuse)
			Effects->Chaos == GL_TRUE;
	}
}

GLboolean IsOtherPowerUpActive(vector<PowerUp> &powerups, string type)
{
	for(PowerUp &powerup : powerups)
	{
		if(powerup.Type == type)
			return powerup.Activated;
	}
	return GL_FALSE;
}


void Game::UpdatePowerUps(GLfloat dt)
{
	for(PowerUp &powerup : this->PowerUps)
	{
		powerup.position += powerup.velocity * dt;
		if(powerup.Activated)
		{
			//每个吃到的道具时间都在减
			powerup.Durations -= dt;
			if(powerup.Durations <= 0)
			{
				powerup.Activated = GL_FALSE;
				if(powerup.Type == "sticky")	
				{
					if(!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{
						Ball->Sticky = GL_FALSE;
						Player->color = glm::vec3(1.0f);
					}
				}
				else if(powerup.Type == "pass-through")	
				{
					if(!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->Sticky = GL_FALSE;
						Ball->color = glm::vec3(1.0f);
					}
				}
				if(powerup.Type == "confuse")	
				{
					if(!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = GL_FALSE;
					}
				}
				if(powerup.Type == "chaos")	
				{
					if(!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	//看上去很唬人的一个东西，实际上是移除已经destroyed和not activated的道具
	this->PowerUps.erase(remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp &powerUp) { return powerUp.destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}