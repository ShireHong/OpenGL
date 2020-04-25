#include "game_level.h"

#include<fstream>
#include<sstream>

void GameLevel::Load(const char* file, GLuint levelWidth, GLuint levelHeight)
{
	this->Bricks.clear();

	ifstream in(file);
	string line;
	vector<vector<GLuint>> tileData;
	if(in)
	{
		while(getline(in,line))
		{
			istringstream is(line);
			GLuint tileCode;
			vector<GLuint> row;
			while(is>>tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if(tileData.size()>0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.destroyed)
            tile.Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
        if (!tile.issolid && !tile.destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

void    GameLevel::init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width  = tileData[0].size();

	GLuint unit_w = levelWidth/width;
	GLuint unit_h = levelHeight/height;

	for(GLuint x = 0; x < height; x++)
	{
		for(GLuint y = 0; y < width; y++)
		{
			if(tileData[x][y] == 1)
			{
				glm::vec2  pos(y*unit_w, x*unit_h);
				glm::vec2  size(unit_w , unit_h);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.issolid = GL_TRUE;
				Bricks.push_back(obj);
			}
			else if(tileData[x][y] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if(tileData[x][y] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[x][y] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[x][y] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[x][y] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2  pos(y*unit_w, x*unit_h);
				glm::vec2  size(unit_w , unit_h);
				this->Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
			}

		}
	}
}	

