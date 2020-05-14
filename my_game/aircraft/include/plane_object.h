#ifndef  _BALL_OBJECT_H
#define  _BALL_OBJECT_H
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


/*
此结构体生成一个球的类，并且继承了游戏大结构体的类
*/

using namespace std;

#define  TIME_CONST  400

const glm::vec2 L_SIZE(20,60);
const glm::vec2 L_VELOCITY(0.0f, 400.0f);

const glm::vec2 C_SIZE(20,20);
const glm::vec2 C_VELOCITY(0.0f, 400.0f);

struct Bullet
{
	glm::vec2 Position;
	glm::vec2 Siz;
	glm::vec2 Velocity;
	glm::vec4 Color;
	GLfloat   Rotation;
	GLfloat   Life;

	Bullet(glm::vec2 pos, glm::vec2 sz, glm::vec2 vel)
		: Position(pos), Siz(sz), Velocity(vel), Color(1.0f), Life(0.0f)
	{

	}
};


class PlaneObject
{
public:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 velocity;
	glm::vec3 color;
	float rotation;

	string    Role;
	GLuint    PlaneLife;
	GLboolean Destroyed;
	vector<Bullet> bullets;
	GLfloat     IntervalTime;
	texture2D 	sprite;
	texture2D	bullet_tex;
	texture2D   plane_gas;

	PlaneObject();
	PlaneObject(glm::vec2 pos, glm::vec2 sz, glm::vec2 vel,string role,texture2D sprite ,texture2D b_tex,texture2D gas,glm::vec3 clr = glm::vec3(1.0f));

	virtual void Draw(SpriteRenderer &renderer);
	virtual void Fire(GLfloat dt, GLuint window_width, GLuint window_height);
    virtual void Reset(glm::vec2 pos, glm::vec2 vel);
    virtual void Move(GLfloat dt, GLuint window_width, GLuint window_height);
	
};
#if 0
class PlaneObject : public GameObject
{
public:
	 // 球的状态 
    GLfloat   Radius; //半径

    PlaneObject();
    PlaneObject(glm::vec2 pos, glm::vec2 size, glm::vec2 vel, texture2D sprite, texture2D tex);

    glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height);
    void      Fire(GLfloat dt);
    void      Reset(glm::vec2 pos, glm::vec2 vel);
    void	  FireRender(SpriteRenderer &renderer);

private:
	vector<Bullet> bullets;
	texture2D	bullet_tex;
	GLfloat     IntervalTime;
};
#endif

#endif