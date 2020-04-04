#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


/*
定义游戏中的各元素的位置，大小，纹理，速度和颜色，包括所有的变量，并且可以调用spriterender渲染这些元素。
*/

class GameObject
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;
	glm::vec3 color;
	float rotation;
	bool  issolid;
	bool  destroyed;

	texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer &renderer);
};


#endif 