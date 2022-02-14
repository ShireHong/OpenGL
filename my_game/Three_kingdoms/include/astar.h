#ifndef _ASTAR_H
#define _ASTAR_H

#include <vector>
#include <tuple>


#define MANCOSTS(sx,sy,ex,ey) (abs(sx-ex)+abs(sy-ey))

typedef std::tuple<int, int, int> Tilesize;

enum Direction{
	RIGHT,
	DOWN,
	LEFT,
	UP,
};
struct PathNode{
	Direction dir;
	int map_x;
	int map_y;
};

void astarinit(Tilesize &size);

Tilesize gettilesize(const std::string name);

void loadtilemap(const std::string name);

int astarfindpath(int sx, int sy, int tx, int ty, int x_range, int y_range, char* path, std::vector<PathNode> &pathset);

//void storepathnode(std::vector<PathNode> &pathset, int sx, int sy, char *path, int path_len);

#endif