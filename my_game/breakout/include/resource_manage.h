#ifndef _RESOURCE_MANAGE_H
#define _RESOURCE_MANAGE_H

#include<map>
#include<string.h>

#include <GL/glew.h>

#include "texture.h"
#include "shader.h"

using namespace std;

class ResourceManager
{
public:
	static map<string, shader>  	shaders;
	static map<string, texture2D>	textures;
	static shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, string name);
	static shader  GetShader(string name);
	static texture2D LoadTexture(const char *file, bool alpha, string name);
	static texture2D GetTexture(string name);

	static void clear();
private:
	ResourceManager(){}
	 // Loads and generates a shader from file
    static shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // Loads a single texture from file
    static texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif