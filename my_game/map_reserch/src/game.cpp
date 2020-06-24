#include <algorithm>
#include <sstream>

#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
#include "particle.h"


#include <iostream>
#include <stdlib.h>


SpriteRenderer  	*Renderer;


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

    

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

  
}

void Game::ProcessInput(float dt)
{
	
}



void Game::UpDate(float dt)
{
	
}


void Game::Render(float dt)
{
	
	
}


