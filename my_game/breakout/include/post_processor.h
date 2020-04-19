#ifndef _POST_PROCESSOR_H
#define _POST_PROCESSOR_H


#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"

//后期
class PostProcessor{
public:
	shader 		ppshader;
	texture2D	tex;
	GLuint      Width,Height;

	GLboolean 	Confuse,Chaos,Shake;

	PostProcessor(shader shr, GLuint width, GLuint height);
	
	void BeginRender();

	void EndRender();

	void Render(GLfloat time);
private:
	GLuint MSFBO,FBO;

	GLuint RBO;

	GLuint VAO;

	void initRenderData();
};

#endif