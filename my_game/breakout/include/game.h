#ifndef _GAME_H
#define _GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


enum game_state{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game{
public:
	game_state  state;
	bool         keys[1024];
	unsigned int width;
	unsigned int height;
	Game(unsigned int w, unsigned int h);
	~Game(); 
	void init();
	void process_input(float dt);
	void update(float dt);
	void render();
};

#endif