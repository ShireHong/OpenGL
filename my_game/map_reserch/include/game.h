#ifndef _GAME_H
#define _GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "plane_object.h"
#include <vector>
#include <tuple>


#define MAP_VELOCITY   80


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

enum Direction{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef tuple<GLboolean, Direction, glm::vec2> Collision;


// Initial size of the player paddle
const glm::vec2 PLANE_SIZE(120, 80);
// Initial velocity of the player paddle
const glm::vec2 PLANE_VELOCITY(300, 300);

const glm::vec2 ENEMY_VELOCITY(0,150);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.

class Game{
public:
	GameState  			State;
	GLboolean         	Keys[1024];
	GLboolean 			KeysProcessed[1024];
	unsigned int 		Width;
	unsigned int 		Height;

	GLfloat             Map_y;

	vector<PlaneObject> enemys;

	

	Game(unsigned int w, unsigned int h);
	~Game(); 
	void Init();
	void ProcessInput(float dt);
	void UpDate(float dt);
	void Render(float dt);	

	void EnemyUpdate(float dt);
	void DoCollisions();
};

#endif