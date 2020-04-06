#include "particle.h"

ParticleGenerator::ParticleGenerator(shader shr, texture2D tex, GLuint amount)
	: shr(shr),tex(tex),amount(amount)
{
	this->init();
}


void ParticleGenerator::Update(GLfloat dt, GameObject &obj, GLuint newParticles, glm::vec2 offset)
{
	for(GLuint i = 0; i < newParticles;i++)
	{
		GLuint unused = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unused], obj, offset);
	}

	for(GLuint i = 0; i < this->amount; i++)
	{
		Particle &p = this->particles[i];
		p.Life -= dt;
		if(p.Life > 0)
		{
			p.Position -= p.Velocity * dt; //一直减小位置
			p.Color.a  -= dt * 2.5;
		}
	}
}

void ParticleGenerator::Draw()
{
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	 this->shr.use();
	 for(Particle p : this->particles)
	 {
	 	if(p.Life > 0)
	 	{
	 		this->shr.set_vec2("offset", p.Position);
	 		this->shr.set_vec4("color", p.Color);
	 		this->tex.bind();
	 		glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
	 	}
	 }
	 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//这里使用了一个巧妙的方法，把一行数据一次写入，因为4个数可以表示为rgba或者xyzw
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	//初始化一些实例
	for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

GLfloat lastusedparticles = 0 ;
GLuint ParticleGenerator::firstUnusedParticle()
{
	//从当前向后查找
	for(GLuint i = lastusedparticles; i < this->amount;i++)
	{
		if(this->particles[i].Life <= 0)
		{
			lastusedparticles = i;
			return i;
		}
	}

	//从头开始找，直到上一次的值
	for(GLuint i = 0; i < lastusedparticles; i++)
	{
		if(this->particles[i].Life <= 0)
		{
			lastusedparticles = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastusedparticles = 0;
    return 0;
}


void ParticleGenerator::respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset )
{
	GLfloat random = (rand() % 100-50)/10;
	GLfloat rcolor  = 0.5 + ((rand() % 100)/100);
	particle.Position = obj.position + random + offset;
	particle.Color = glm::vec4(rcolor, rcolor, rcolor, 1.0f);
	particle.Life = 1.0f;
    particle.Velocity = obj.velocity * 0.1f;
}







