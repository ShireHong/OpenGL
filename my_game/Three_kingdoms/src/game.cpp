#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "resource_manage.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "cursor.h"
#include "player.h"
#include "astar.h"
#include <GL/glu.h>


using namespace std;

SpriteRenderer  	*Renderer;
Cursor              *Arrow;
Player              *Role;
TextRenderer        *Text;

bool firstmouse_a = true;

/*滑动窗口*/

Game::Game(unsigned int w, unsigned int h)
		: State(GAME_MENU), Keys(), Width(w), Height(h),cursorx(w/2),cursory(h/2)
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

	Tilesize tilesize;

    tilesize = gettilesize("resource/file/13-1.csv");

    astarinit(tilesize);

	loadtilemap("resource/file/13-1.csv");
	
	/*shader*/
	ResourceManager::LoadShader("resource/shader/sprite.vs", "resource/shader/sprite.fs", nullptr, "sprite" );
	ResourceManager::LoadShader("resource/shader/particle.vs", "resource/shader/particle.fs", nullptr, "particle" );
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 	0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").use().set_int("image", 0);
    ResourceManager::GetShader("sprite").set_mat4("projection", projection);
	
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	/*load map */
	/*第一个图的alph参数必须配置成GL_FALSE*/
	ResourceManager::LoadTexture("resource/texture/map/13-1.jpg", GL_FALSE, "map");

	/*load arrow*/	
	ResourceManager::LoadTexture("resource/texture/cursor.png", GL_TRUE, "arrow");    

	glm::vec2 arrowpos = glm::vec2(this->Width / 2 /*- ARROW_SIZE.x / 2*/,  this->Height / 2 /*- ARROW_SIZE.y*/);
	Arrow    = new Cursor(arrowpos, ARROW_SIZE, ResourceManager::GetTexture("arrow"), 
							(float)(this->Width / 2), (float)(this->Height /2), glm::vec3(1.0f));

	/*load role*/

	ResourceManager::LoadTexture("resource/texture/player/guanyu/down_0.png", GL_TRUE, "down_0");
	
	glm::vec2 rolepos = glm::vec2(100 - ROLE_SIZE.x / 2,  100 - ROLE_SIZE.y/2);
	Role = new Player(rolepos,ROLE_SIZE,ResourceManager::GetTexture("down_0"),glm::vec3(1.0f));

	for(GLuint i = 0;i < 4;i++)
	{
		vector<string> tmp;
		string tex;
		string name;
		for(GLuint j = 0;j < 4;j++)
		{
			switch(i)
			{
				case RIGHT:
					tex = "resource/texture/player/guanyu/right_"+to_string(j)+".png";
					name = "right_"+to_string(j);
					break;
				case DOWN:
					tex = "resource/texture/player/guanyu/down_"+to_string(j)+".png";
					name = "down_"+to_string(j);
					break;
				case LEFT:
					tex = "resource/texture/player/guanyu/left_"+to_string(j)+".png";
					name = "left_"+to_string(j);
					break;
				case UP:
					tex = "resource/texture/player/guanyu/up_"+to_string(j)+".png";
					name = "up_"+to_string(j);
					break;
				default:
					break;
					
			}			
			tmp.push_back(name);
			ResourceManager::LoadTexture(tex.c_str(), GL_TRUE, name);
		}		
		Role->walk.push_back(tmp);
	}
	/*load fonts*/	
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("resource/fonts/arial.ttf", 24);
  
}

int select_point = 0;

void Game::ProcessInput(float dt)
{
	int hits;
	GLuint selectBuffer[500];
	GLint viewport[4];
	if(this->MouseButton && !this->MouseButtonProcessed)
	{
		#if 0
		this->MouseButtonProcessed = GL_TRUE;
		Role->targetpos.x = this->cursorx;
		Role->targetpos.y = this->cursory;
		#endif
		glGetIntegerv(GL_VIEWPORT,viewport);//获取当前视口坐标参数
        glSelectBuffer(500,selectBuffer);//选择名称栈存放被选中的名称
        glRenderMode(GL_SELECT);//设置当前为 选择模式

        //初始化名称栈
        glInitNames();
        glPushName(0);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        gluPickMatrix(this->cursorx,viewport[3]-this->cursory,20,20,viewport);//创建用于选择的投影矩阵栈
        //gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT);//设置投影方式
       // draw(GL_SELECT);//绘制场景
        glPopMatrix();
        glFlush();
        hits = glRenderMode(GL_RENDER);
        glMatrixMode(GL_MODELVIEW);
		unsigned int i,j;
	    GLuint name;
		cout<<hits<<endl;
	    for(i = 0; i < hits; i++)
	    {
	        name = selectBuffer[3+i*4];
	        select_point = name;
	        cout<<"第"<<name<<"个点"<<endl;
	    }
		this->MouseButtonProcessed = GL_TRUE;
	}
}

void Game::ProcessMouseMove(float xpos, float ypos)
{
	this->cursorx = xpos ;
	this->cursory = ypos ;	
	
	Arrow->ArrowMove(xpos, ypos,this->Width,this->Height);
}

void Game::UpDate(float dt)
{
	Role->Action(dt);
}


void Game::Render(float dt)
{	
	Renderer->DrawSprite(ResourceManager::GetTexture("map"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);	
	
	Role->Draw(*Renderer);
	Arrow->Draw(*Renderer);

	std::stringstream ss;
	ss<<Arrow->position.x<<","<<Arrow->position.y;
	Text->RenderText("Position:" + ss.str(), this->Width-200, this->Height-200, 1.0f, glm::vec3(0.0,1.0,0.0));
}


