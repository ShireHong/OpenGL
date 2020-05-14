#ifndef _SPRITE_RENDERER_H
#define _SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

/*
根据读取的着色器片段和顶点片段，结合纹理进行渲染，所有元素都是通过这里实现渲染（draw）
着色器片段和顶点片段可以共用，所以在这个游戏中看到这个着色器一般不变
*/


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