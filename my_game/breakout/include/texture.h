#ifndef  _TEXTURE_H
#define  _TEXTURE_H

#include <GL/glew.h>


/*
纹理相关的渲染都是通过这个结构体
*/


class texture2D{
public:
	unsigned int ID;
	unsigned int width;
	unsigned int height;
	unsigned int internal_format;
	unsigned int image_format;
	unsigned int wrap_s;
	unsigned int wrap_t;
	unsigned int filter_min;
	unsigned int filter_max;

	texture2D();

	void generate(unsigned int w, unsigned int h, unsigned char* data);

	void bind() const;
};

#endif