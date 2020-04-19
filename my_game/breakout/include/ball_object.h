#ifndef  _BALL_OBJECT_H
#define  _BALL_OBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"

/*
此结构体生成一个球的类，并且继承了游戏大结构体的类
*/


class BallObject : public GameObject
{
public:
	 // 球的状态 
    GLfloat   Radius; //半径
    GLboolean Stuck;
    GLboolean Sticky, PassThrough;

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 vel, texture2D sprite);

    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void      Reset(glm::vec2 pos, glm::vec2 vel);
};

#endif