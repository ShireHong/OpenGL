#ifndef _SPRITE_RENDERER_H
#define _SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
public:
	SpriteRenderer(const shader &shr);
	
	~SpriteRenderer();

	void DrawSprite(const texture2D &tex, glm::vec2 pos, glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f) );
private:
	shader sh;
	unsigned int quadVAO;
	void initRenderData();
};



#endif