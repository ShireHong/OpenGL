#include <algorithm>
#include <sstream>

#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
//#include "particle.h"
#include "text_renderer.h"
#include "cursor.h"
#include "player.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define MAP_COLUNM_MAX_INDEX 	13
#define MAP_ROW_MAX_INDEX 	9


#define MAP_ROW       		3
#define MAP_COLUMN       	4


SpriteRenderer  	*Renderer;
Cursor              *Arrow;
Player              *Role;
TextRenderer        *Text;

bool firstmouse_a = true;

/*滑动窗口*/

GLint SlidewinLeft  	= 4;
GLint SlidewinUp    	= 2;





Game::Game(unsigned int w, unsigned int h)
		: State(GAME_MENU), Keys(), Width(w), Height(h),MapRow(MAP_ROW),MapColumn(MAP_COLUMN),cursorx(w/2),cursory(h/2)
{

}

Game::~Game()
{
	delete Renderer;
	delete Arrow;
	delete Role;
	delete Text;
}

void Game::Init()
{
	
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	ResourceManager::LoadShader("resource/shader/particle.vs", "resource/shader/particle.fs", nullptr, "particle" );
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 	0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);

	/*load map*/
	for(GLuint i=0;i<MAP_ROW_MAX_INDEX;++i)
	{
		for(GLuint j=0;j<MAP_COLUNM_MAX_INDEX;++j)
		{
			/*
			char idex[50]={0} ;
			memset(idex,0,50);
			sprintf(idex,"resource/texture/map/%d_%d.jpg",i,j);
			*/
			string idex = "resource/texture/map/"+to_string(i)+"_"+to_string(j)+".jpg";
			string name = "map_"+to_string(i)+"_"+to_string(j);
			ResourceManager::LoadTexture(idex.c_str(), GL_FALSE, name);
		}
	}
	
	ResourceManager::LoadTexture("resource/texture/cursor.png", GL_TRUE, "arrow");
	ResourceManager::LoadTexture("resource/texture/player/stop_0.png", GL_TRUE, "stop_0");
	
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	glm::vec2 arrowpos = glm::vec2(this->Width / 2 /*- ARROW_SIZE.x / 2*/,  this->Height / 2 /*- ARROW_SIZE.y*/);
	Arrow    = new Cursor(arrowpos, ARROW_SIZE, ResourceManager::GetTexture("arrow"), 
							(float)(this->Width / 2), (float)(this->Height /2), glm::vec3(1.0f));
	
	
	glm::vec2 rolepos = glm::vec2(this->Width / 2 - ROLE_SIZE.x / 2,  this->Height / 2 - ROLE_SIZE.y/2);
	Role = new Player(rolepos,ROLE_SIZE,ResourceManager::GetTexture("stop_0"),glm::vec3(1.0f));
	
	for(GLuint i = 0;i < 6;i++)
	{
		string idex = "resource/texture/player/stop_"+to_string(i)+".png";
		Role->stop[i] = "stop_"+to_string(i);		
		ResourceManager::LoadTexture(idex.c_str(), GL_TRUE, Role->stop[i]);
	}
	for(GLuint i = 0;i < 6;i++)
	{
		string idex = "resource/texture/player/walk_left_"+to_string(i)+".png";
		Role->walkleft[i] = "walk_left_"+to_string(i);		
		ResourceManager::LoadTexture(idex.c_str(), GL_TRUE, Role->walkleft[i]);
	}
	
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("resource/fonts/arial.ttf", 24);
  
}

void Game::ProcessInput(float dt)
{
	if(this->MouseButton && !this->MouseButtonProcessed)
	{
		this->MouseButtonProcessed = GL_TRUE;
		cout<<this->cursorx<<"---"<<this->cursory<<endl;
		Role->targetpos.x = this->cursorx;
		Role->targetpos.y = this->cursory;
	}
}

void Game::ProcessMouseMove(float xpos, float ypos)
{
	this->cursorx = xpos ;
	this->cursory = ypos ;	
	
	Arrow->ArrowMove(xpos, ypos,this->Width,this->Height);
}

GLuint cnt =0 ;
void Game::UpDate(float dt)
{
	GLint xmod = this->cursorx/300;
	GLint ymod = this->cursory/300;

	//cout<<xmod<<"---"<<ymod<<endl;
	cnt++;
	if(cnt == 300)
	{
		
	    this->MapGroup.clear();
		cnt=0;
#if 0
		if(xmod != 2)
		{
			SlidewinLeft 	+= xmod - 2;
		}
		if(ymod != 1)
		{
			SlidewinUp 		+= ymod - 1;
		}

		//cout<<SlidewinLeft<<"++++++"<<SlidewinUp<<endl;
		if(SlidewinLeft > 9)
		{
			SlidewinLeft = 9;
		}
		else if(SlidewinLeft < 0)
		{
			SlidewinLeft = 0;
		}
		if(SlidewinUp > 6)
		{
			SlidewinUp = 6;
		}
		else if(SlidewinUp < 0)
		{
			SlidewinUp = 0;
		}
#endif
		for(GLuint i = 0; i < this->MapRow ; i++)
		{
			for(GLuint j = 0; j<this->MapColumn;j++)
			{
				string name = "map_"+to_string(SlidewinUp+i)+"_"+to_string(SlidewinLeft+j);
				glm::vec2 pos;
				pos.x = j * 300;
				pos.y = i * 300;
				this->MapGroup.push_back(MapBlock(name,pos));
			}
		}
	}

	Role->Action(dt);
}


void Game::Render(float dt)
{
	//Renderer->DrawSprite(ResourceManager::GetTexture("map_0_2"), glm::vec2(0, 0), glm::vec2(300, 300 ), 0.0f);
	//Renderer->DrawSprite(ResourceManager::GetTexture("map_0_2"), glm::vec2(0, 0), 
	//						glm::vec2(this->Width, this->Height ), 0.0f);
	//if(cnt == 20)
	{		
		for(MapBlock &block : this->MapGroup)
	    {
	    	//cout<<block.Name<<endl;
			//cout<<block.Position.x<<" "<<block.Position.y<<endl;
	    	Renderer->DrawSprite(ResourceManager::GetTexture(block.Name), block.Position, 
								glm::vec2(300, 300 ), 0.0f);
	    }
	    //cnt = 0;
	}
	
	Role->Draw(*Renderer);
	Arrow->Draw(*Renderer);

	std::stringstream ss;
    //ss<<this->cursorx<<","<<this->cursory;
	ss<<Arrow->position.x<<","<<Arrow->position.y;
	Text->RenderText("Position:"+ ss.str() , this->Width-200, this->Height-200, 1.0f, glm::vec3(0.0,1.0,0.0));
}


