#ifndef _GAME_LEVEL_H
#define _GAME_LEVEL_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manage.h"

using namespace std;


/*
游戏关卡的结构体，包括所有的砖块，加载一些关卡，并渲染这些关卡
*/

class GameLevel
{
public:

	vector<GameObject> Bricks;

	GameLevel(){}

	void Load(const char* file, GLuint levelWidth, GLuint levelHeight);


	void      Draw(SpriteRenderer &renderer);
    // Check if the level is completed (all non-solid tiles are destroyed)
    GLboolean IsCompleted();
private:
    // Initialize level from tile data
    void      init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);	
	
};


#endif