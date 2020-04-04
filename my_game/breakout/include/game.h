#ifndef _GAME_H
#define _GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "game_level.h"

#include <vector>
#include <tuple>

/*
此结构体是整个游戏控制的类，初始化游戏相关的关卡，挡板，球，加载这些元素的shader和纹理
控制游戏的一些输出，通过按键变化来控制游戏特效。
*/
using namespace std;

enum game_state{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef tuple<GLboolean, Direction, glm::vec2> Collision;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.

class Game{
public:
	game_state  		state;
	bool        		keys[1024];
	unsigned int 		width;
	unsigned int 		height;
	vector<GameLevel> 	Levels;
    GLuint           	Level;


	Game(unsigned int w, unsigned int h);
	~Game(); 
	void init();
	void process_input(float dt);
	void update(float dt);
	void render();
	void DoCollisions();
	void ResetLevel();
	void ResetPlayer();
};

#endif