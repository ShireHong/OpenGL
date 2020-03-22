#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

shader &shader::use()
{
	glUseProgram(this->ID);
	return *this;
}

void shader::compile(const char* vertexsource, const char * fragsource, const char* geometrysource )
{
	unsigned int svertex, sfrag, gshader;
	//定点着色器
	svertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(svertex, 1, &vertexsource, NULL);
	glCompileShader(svertex);
	check_compile_errors(svertex, "VERTEX");
	//片段着色器
	sfrag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sfrag, 1, &fragsource, NULL);
	glCompileShader(sfrag);
	check_compile_errors(sfrag, "FRAGMENT");

	if(geometrysource != nullptr)
	{
		gshader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gshader, 1, &geometrysource, NULL);
		glCompileShader(gshader);
		check_compile_errors(gshader, "GEOMETRY");
	}
	this->ID = glCreateProgram();
	glAttachShader(this->ID, svertex);
	glAttachShader(this->ID, sfrag);
	if(geometrysource != nullptr)
		glAttachShader(this->ID, gshader);
	glLinkProgram(this->ID);
	check_compile_errors(this->ID, "PROGRAM");

	glDeleteShader(svertex);
    glDeleteShader(sfrag);
    if (geometrySource != nullptr)
        glDeleteShader(gshader);
}

void Shader::set_float(const char *name, float value, bool useshader)
{
    if (useshader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::set_int(const char *name, unsigned int value, bool useshader)
{
    if (useshader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::set_vec2(const char *name, float x, float y, bool useshader)
{
    if (useshader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::set_vec2(const char *name, const glm::vec2 &value, bool useshader)
{
    if (useshader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::set_vec3(const char *name, float x, float y, float z, bool useshader)
{
    if (useshader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::set_vec3(const char *name, const glm::vec3 &value, bool useshader)
{
    if (useshader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::set_vec4(const char *name, float x, float y, float z, float w, bool useshader)
{
    if (useshader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::set_vec4(const char *name, const glm::vec4 &value, bool useshader)
{
    if (useshader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::set_mat2(const char *name, const glm::mat2 &matrix, bool useshader)
{
    if (useshader)
        this->use();
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::set_mat3(const char *name, const glm::mat3 &matrix, bool useshader)
{
    if (useshader)
        this->use();
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::set_mat4(const char *name, const glm::mat4 &matrix, bool useshader)
{
    if (useshader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::check_compile_errors(unsigned int obj, const char * type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(obj, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(obj, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(obj, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}