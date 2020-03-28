#ifndef _GAME_LEVEL_H
#define _GAME_LEVEL_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

using namespace std;

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