#ifndef _GAME_H
#define _GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <tuple>


#define MAP_VELOCITY   80


const glm::vec2 ARROW_SIZE(20, 20);

/*
此结构体是整个游戏控制的类，初始化游戏相关的关卡，挡板，球，加载这些元素的shader和纹理
控制游戏的一些输出，通过按键变化来控制游戏特效。
*/
using namespace std;

enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};



struct MapBlock{
	string    Name;
	glm::vec2 Position;
	

	MapBlock(string name, glm::vec2 pos)
		: Name(name),Position(pos)
	{

	}
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.

class Game{
public:
	GameState  			State;
	GLboolean         	Keys[1024];
	GLboolean 			KeysProcessed[1024];
	GLboolean         	MouseButton;
	GLboolean 			MouseButtonProcessed;
	unsigned int 		Width;
	unsigned int 		Height;
	
	GLfloat             cursorx;
	GLfloat             cursory;

	Game(unsigned int w, unsigned int h);
	~Game(); 
	void Init();
	void ProcessInput(float dt);
	void UpDate(float dt);
	void Render(float dt);	
    void ProcessMouseMove(float xpos, float ypos);
};

#endif
