在做纹理这一块的时候遇到几个问题：
1，#include<stb_image.h>编译的时候要报错，这个文件是放到当前目录下的，所以要用#include "stb_image.h"。
2，即使这个可行了，会发现里面的函数还是无法正确调用，需要加上一个宏，
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
3，关于opengl教程中使用的filesystem不支持的问题，选择直接使用，不要使用filesystem
filesystem是c++底层缺少boost库，需要编译使用

编译命令：
g++ -std=c++17 test.cpp -o test   -lglew32 -lglut32 -lopengl32 -lglfw3 -lglad

