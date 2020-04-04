#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "game_level.h"
#include <iostream>


SpriteRenderer  *Renderer;
GameObject      *Player;
BallObject      *Ball;

Game::Game(unsigned int w, unsigned int h)
		: state(GAME_ACTIVE), keys(), width(w), height(h) 
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::init()
{
	
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 
		0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("resource/texture/background.png", GL_FALSE, "background");
    ResourceManager::LoadTexture("resource/texture/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("resource/texture/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("resource/texture/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("resource/texture/paddle.png", GL_TRUE, "paddle");


    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

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
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
        ResourceManager::GetTexture("face"));

}

void Game::process_input(float dt)
{
	if(this->state == GAME_ACTIVE)
	{
		GLfloat vel = PLAYER_VELOCITY *dt;
		if(this->keys[GLFW_KEY_A] == GL_TRUE)
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
}

void Game::update(float dt)
{
	Ball->Move(dt,this->width);
	this->DoCollisions();
	if (Ball->position.y >= this->height) // Did ball reach bottom edge?
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)
    	this->Levels[0].Load("levels/one.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("levels/two.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("levels/three.lvl", this->width, this->height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("levels/four.lvl", this->width, this->height * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->size = PLAYER_SIZE;
    Player->position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Ball->Reset(Player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


void Game::render()
{
	/*Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	*/
	if(this->state == GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);

        Ball->Draw(*Renderer);
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

void Game::DoCollisions()
{
	for(GameObject &box : this->Levels[this->Level].Bricks)
	{
		if(!box.destroyed) 
		{			
			Collision  coll_res = CheckCollision(*Ball,box);
			if(get<0>(coll_res))
			{
				if(!box.issolid)
					box.destroyed = GL_TRUE;

				Direction dir  = get<1>(coll_res);
				cout<<"dir = "<<dir<<endl;
				glm::vec2 diff = get<2>(coll_res);
				cout<<"diff.x = "<<diff.x<<endl;
				cout<<"diff.y = "<<diff.y<<endl;
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
	#if 1
	Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->position.x + Player->size.x / 2;
        GLfloat distance = (Ball->position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->velocity;
        Ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        Ball->velocity.y = -Ball->velocity.y;
        Ball->velocity = glm::normalize(Ball->velocity) * glm::length(oldVelocity);
        //Ball->velocity.y = -1 * abs(Ball->velocity.y);
    } 
    #endif
}
