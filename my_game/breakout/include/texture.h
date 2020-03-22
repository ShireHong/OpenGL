#ifndef  _TEXTURE_H
#define  _TEXTURE_H

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

	void generate(unsigned int w, unsigned int h, unsigned char* date);

	void bind() const;
}

#endif