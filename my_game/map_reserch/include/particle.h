#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"
#include "plane_object.h"

using namespace std;

struct Particle{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	GLfloat   Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f)
	{

	}
};


class ParticleGenerator
{
public:
	ParticleGenerator(shader shr, texture2D tex, GLuint amount);
	
	void Update(GLfloat dt, PlaneObject &obj, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f));

	void Draw();
private:
	vector<Particle> particles;

	GLuint amount;

	shader shr;

	texture2D tex;

	GLuint VAO;

	void init();

	GLuint firstUnusedParticle();

	void respawnParticle(Particle &particle, PlaneObject &obj, glm::vec2 offset = glm::vec2(0.0f));
};


#endif