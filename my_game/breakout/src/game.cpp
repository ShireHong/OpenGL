#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"


SpriteRenderer  *Renderer;

Game::Game(unsigned int w, unsigned int h)
		: state(GAME_ACTIVE), keys(), width(w), height(h) 
{

}

Game::~Game()
{

}

void Game::init()
{
	
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 
		0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("resource/texture/awesomeface.png", GL_TRUE, "face");
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
}

void Game::process_input(float dt)
{

}

void Game::update(float dt)
{

}

void Game::render()
{
	Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}