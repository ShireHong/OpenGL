#ifndef _TEXT_RENDERER_H
#define _TEXT_RENDERER_H

#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

using namespace std;

struct Character{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};


class TextRenderer
{
public:

	map<GLchar, Character> Characters;

	shader TextShader;

	TextRenderer(GLuint width, GLuint height);

	void Load(string font, GLuint fontsize);

	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color=glm::vec3(1.0f));

private:
	GLuint VAO,VBO;
	
};

#endif