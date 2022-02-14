#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "astar.h"
using namespace std;


#define ONETILE 8



struct Open {
	int center_x;     /// X coordinate
	int center_y;     /// Y coordinate
	int ox;           /// Offset into matrix x
	int oy;           /// Offset into matrix y
	int costs;        /// complete costs to goal
};

struct Node {
	char dir;           /// Direction for trace back
	char isgoal;        /// is this point in the goal
	int  costfromstart; /// Real costs to reach this point
	int  tile_center_x;
	int  tile_center_y;
    int  isobstacle;
};

struct direction{
	int x;
	int y;
};

struct Close{
	int offset_x;
	int offset_y;
};

direction nodedir[] = {
						{1,0},{0,1},{-1,0},{0,-1},/*right down left up*/
            			{-1,-1},{-1,1},{1,-1},{1,1},{0,0}
            		};

static Node ** astarmatrix = NULL;
static Open *  openset= NULL;
static Close*  closeset= NULL;
static int     OpenSetSize;
static int     num_in_close = 0;
static int     OpenSetSizeMax = 225;


#define AStarFindMinimum() (OpenSetSize - 1)

void astarinit(Tilesize &size)
{
	int row = get<0>(size);
	int column = get<1>(size);
	int all = get<2>(size);
    astarmatrix = (Node**)malloc(row*sizeof(Node*));
    for(int i=0; i< row; i++)
   	{
    	astarmatrix[i] = (Node *)malloc(sizeof(Node)*column);
   	}
    if(astarmatrix == NULL)
    {
    	cout<<"malloc fail!"<<endl;
    }
    for(int i = 0;i<row;i++)
    {
    	for(int j = 0;j<column;j++)
    	{
    		astarmatrix[i][j].isobstacle = 1;
    		astarmatrix[i][j].isgoal = 0;
    	}
    }
	openset = new Open[all/4];
	//openset = (Open*)malloc(all/4*sizeof(Node));
	memset(openset, 0, all/4 *sizeof(Open));
	closeset = new Close[all/4];
	//closeset = (Close*)malloc(all/4*sizeof(Close));
	memset(closeset, 0, all/4 *sizeof(Close));
	OpenSetSize = 0;
	OpenSetSizeMax = all/4;
	num_in_close = 0;
}


/**
** Remove the minimum from the open node set
*/
static void AStarRemoveMinimum(int pos)
{
	if(pos > OpenSetSize -1)
	{
		printf("pos is out of range!\n");
		return;
	}

	OpenSetSize--;
}


static int detectcollision(int x, int y)
{
	if(astarmatrix[x][y].isobstacle)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


static int astaraddnode(int o_x, int o_y, int cost)
{
	int index = 0;
	int end = OpenSetSize;
    int mid,midcost;

    if(OpenSetSize + 1 >= OpenSetSizeMax)
    {
    	printf("opset max size!\r\n");
    	return -1;
    }
    
    while(index < end )
    {
    	mid 	= (index+end)>>1;
    	midcost = openset[mid].costs;
    	/*因为是从高到低排序，cost值高的排在左边*/
    	if(cost > midcost)
    	{
    		end = mid;
    	}
    	/*当cost值小的时候，比较当前索引的位置，若与中间值相等，则往后排，不相等就替换
    	*然后继续比较。
    	*/
    	else if(cost < midcost)
    	{
    		if(index == mid)
    		{
    			/*若没有最小值，通过这一步往后加，加到最后一位，直接推出循环*/
    			index++;
    		}
    		else
    		{
    			index=mid;
    		}
    	}
    	/*若相等，则比较结束，更新一下*/
    	else
    	{
    		index=mid;
    		end=mid;
    	}
    }
    if(index < OpenSetSize)
    {    	
    	/*将index以及后面的值替换到index+1这个位置*/
    	memmove(&openset[index+1], &openset[index],(OpenSetSize-index)*sizeof(Open));
    }

    openset[index].costs = cost;
    openset[index].ox = o_x;
    openset[index].oy = o_y;
    ++OpenSetSize;
 	return 0;
}

static void replacenode(int index, int cost)
{
	Open node;

	node = openset[index];
	OpenSetSize--;

	memmove(&openset[index], &openset[index+1],(OpenSetSize-index)*sizeof(Open));
	astaraddnode(node.ox, node.oy, node.costs);
}

static int addcloseset(int x, int y)
{
	closeset[num_in_close].offset_x = x;
	closeset[num_in_close].offset_x = x;
	num_in_close ++;
}

static int findincloseset(int x, int y)
{
	for(int i = 0;i < num_in_close;i++)
	{
		if(closeset[i].offset_x == x && closeset[i].offset_y == y)
		{
			return 1;
		}
	}
	return 0;
}

static int findinopenset(int x, int y)
{
	for(int i = 0;i < OpenSetSize;i++)
	{
		if(openset[i].ox == x && openset[i].oy == y)
		{
			return i;
		}
	}
	return -1;
}


static void astarprepare(int ox, int oy)
{
	astarmatrix[ox][oy].isgoal = 1;
}
#if 0
void storepathnode(vector<PathNode> &pathset, int sx, int sy, char *path, int path_len)
{
	int tmp_x,tmp_y;
	PathNode node;
	for(int i=path_len-1; i>=0;i--)
	{
		if(i == path_len-1)
		{
			tmp_x = sx + nodedir[path[i]].x;
			tmp_y = sy + nodedir[path[i]].y;
			//astarmatrix[tmp_x][tmp_y].isobstacle = '#';
		}
		else
		{
			tmp_x +=  nodedir[path[i]].x;
			tmp_y +=  nodedir[path[i]].y;
			//astarmatrix[tmp_x][tmp_y].isobstacle = '#';
		}
		node.dir = path[i];
		node.map_x = tmp_x * 16 + 16/2;
		node.map_y = tmp_y * 16 + 16/2;
		pathset.push_back(node);
	}
}
#endif
/*
 sx sy起始点的坐标
 tx ty终点的坐标
*/
int astarfindpath(int sx, int sy, int tx, int ty, int x_range, int y_range, char* path, vector<PathNode> &pathset)
{
	/*计算主要使用瓦片的坐标，也就是每一个瓦片在图中的位置*/
	int s_o_x = sx / ONETILE;
	int s_o_y = sy / ONETILE;
	int t_o_x = tx / ONETILE;
	int t_o_y = ty / ONETILE;
	int pre_x;
	int pre_y;
	int real_x;
	int real_y;
	int shortest;
	int offset_x;
	int offset_y;/*最小值的瓦片坐标*/
	int cur_x;
	int cur_y;/*遍历4个方向的临时瓦片坐标*/
	int new_cost;
	int i,j;
	int path_len;
#if 0
	s_o_x = sx ;
	s_o_y = sy ;
	t_o_x = tx ;
	t_o_y = ty ;
#endif
	/*起始点和终点都是障碍*/
	if(detectcollision(s_o_x, s_o_y) || detectcollision(t_o_x, t_o_y))
	{
		printf("node is not proper!--detect error\r\n");
		return -1;
	}

	astarprepare(t_o_x, t_o_y);

	/*计算瓦片的中心 x y,统一计算，这个地方还要考虑第一步到中心的距离以及最后一步到中心的距离*/
	//ex = astarmatrix[sx / 32][sy / 32].tile_center_x;
    //ey = astarmatrix[sx / 32][sy / 32].tile_center_y;

    astarmatrix[s_o_x][s_o_y].costfromstart = 1;
    /*将第一个值加入到开放列表中*/
	OpenSetSize = 0;

	astaraddnode(s_o_x, s_o_y,1 + MANCOSTS(sx,sy,tx,ty));

	while(1)
	{
		/*找出开启列表中最小花费的node*/
		shortest = AStarFindMinimum();
		offset_x = openset[shortest].ox;
		offset_y = openset[shortest].oy;
		//printf("%d %d\n",offset_x,offset_y);
		//printf("i:%d\n",astarmatrix[offset_x][offset_y].dir);
	    AStarRemoveMinimum(shortest);
		
	    if(astarmatrix[offset_x][offset_y].isgoal)
	    {
	    	/*到达目标位置*/
	    	t_o_x = offset_x;
	    	t_o_y = offset_y;
	    	break;
	    }

	    addcloseset(offset_x,offset_y);

	    /*获得最小的上一个的node*/
	    pre_x = offset_x - nodedir[astarmatrix[offset_x][offset_y].dir].x;
	    pre_y = offset_y - nodedir[astarmatrix[offset_x][offset_y].dir].y;

	    for(int i = 0;i < 4; i++)
	    {
	    	/*瓦片的坐标*/
	    	cur_x = offset_x + nodedir[i].x;
	    	cur_y = offset_y + nodedir[i].y;
	    	/*实际的坐标，也是中心坐标*/
	    	real_x = cur_x * ONETILE + ONETILE/2;
	    	real_y = cur_y * ONETILE + ONETILE/2;

	    	if(cur_x == pre_x && cur_y == pre_y)
	    	{
	    		continue;
	    	}

	    	if(cur_x < 0 ||cur_x >= x_range || cur_y < 0 || cur_y >= y_range)
	    	{
	    		continue;
	    	}

	    	/*判断是否是墙或者已经在*/
	    	if(detectcollision(cur_x, cur_y) || findincloseset(cur_x, cur_y))
	    	{
	    		continue;
	    	}

	    	new_cost = astarmatrix[offset_x][offset_y].costfromstart + (i < 4) ? 10 : 14;

	    	if(astarmatrix[cur_x][cur_y].costfromstart == 0)
	    	{
	    		/*当前的瓦片没有被访问过*/
	    		astarmatrix[cur_x][cur_y].costfromstart = new_cost;
	    		astarmatrix[cur_x][cur_y].dir = i;
	    		astaraddnode(cur_x, cur_y,astarmatrix[cur_x][cur_y].costfromstart + MANCOSTS(cur_x,cur_y,t_o_x,t_o_y));
	    	}
	    	else if(new_cost < astarmatrix[cur_x][cur_y].costfromstart)
	    	{
	    		astarmatrix[cur_x][cur_y].costfromstart = new_cost;
	    		astarmatrix[cur_x][cur_y].dir = i;
	    		j = findinopenset(cur_x, cur_y);
	    		if(j == -1)
	    		{
	    			astaraddnode(cur_x, cur_y,astarmatrix[cur_x][cur_y].costfromstart + MANCOSTS(cur_x,cur_y,t_o_x,t_o_y));
	    		}
	    		else
	    		{
	    			/*已在现有的openset中，*/
	    			replacenode(j, astarmatrix[cur_x][cur_y].costfromstart + MANCOSTS(cur_x,cur_y,t_o_x,t_o_y));
	    		}
	    	}

	    }
	}
	/*路径搜索完成，开始添加路径*/
	path_len = 0;
	i = 0;
	cur_x = t_o_x;
	cur_y = t_o_y;
	while(s_o_x != cur_x || s_o_y != cur_y)
	{
		i = astarmatrix[cur_x][cur_y].dir;
		//printf("i:%d\n",i);
		cur_x -= nodedir[i].x;
		cur_y -= nodedir[i].y;
		path_len ++;
	}

	cur_x = t_o_x;
	cur_y = t_o_y;
	int temp_path = path_len;
	PathNode node;
	while(s_o_x != cur_x || s_o_y != cur_y)
	{
		i = astarmatrix[cur_x][cur_y].dir;
		cur_x -= nodedir[i].x;
		cur_y -= nodedir[i].y;
		printf("%d %d\n",cur_x,cur_y);
		-- temp_path;
		if(temp_path < path_len)
		{
			path[path_len-temp_path - 1] = i;
		}
		node.dir = (Direction)i/*path[i]*/;
		/*这里需要提供的是目标地址，从当前的值做偏移。*/
		node.map_x = cur_x * ONETILE + ONETILE/2 + nodedir[i].x *ONETILE;
		node.map_y = cur_y * ONETILE + ONETILE/2 + nodedir[i].y *ONETILE;;
		pathset.push_back(node);
	}
	return path_len;
}

Tilesize gettilesize(const string name)
{
	int row = 0,all = 0;
	ifstream in(name);
    string line;
    if(in)
    {
        while(getline(in,line))
        {
            istringstream is(line);
            int tmp;
            string filed;
            char  ch;
            while(is>>tmp)
            {
                is>>ch;
                all++;
            }
            row++;
        }
        return make_tuple(all/row, row, all);
    }
    return make_tuple(0, 0, 0);
}

void loadtilemap(const string name)
{
	ifstream in(name);
    string line;
    int row = 0,column = 0;
    if(in)
    {
        while(getline(in,line))
        {
            istringstream is(line);
            int tile_value;
            row = 0;  
            char  ch;
            while(is>>tile_value)
            {      
                if(tile_value < 100)
                {   
                    astarmatrix[row][column].isobstacle = 0;
                }
                else{
                    astarmatrix[row][column].isobstacle = 1;
                }
                is>>ch;
                row++;
            }
             column++;
        }
    }
    cout<<row<<" "<<column<<endl;
}
#if 0
int main()
{
	int i,j;
	int len;
	Tilesize tilesize;

    tilesize = gettilesize("8-1.csv");

    cout<<get<0>(tilesize)<<" "<<get<1>(tilesize)<<" "<<get<2>(tilesize)<<endl;


    astarinit(tilesize);

	loadtilemap("8-1.csv");

#if 1
	printf(" ");
	for(i = 0;i<96;i++)
	{
		printf("%d", i);
	}
	printf("\n");

    for(j = 0; j < 84; j++)
    {
    	printf("%2d ",j);
    	for(i = 0; i < 96; i++)
    	{
    		printf("%d",astarmatrix[i][j].isobstacle);
    	}
    	printf("\n");
    }
#endif
#if 1
    len = astarfindpath(3,3,9,12,get<0>(tilesize),get<1>(tilesize));
    printf("len = %d\r\n",len);


    printf(" ");
	for(i = 0;i<96;i++)
	{
		printf("%d", i);
	}
	printf("\n");

    for(j = 0; j < 84; j++)
    {
    	printf("%2d ",j);
    	for(i = 0; i < 96; i++)
    	{
    		if(astarmatrix[i][j].isobstacle !=0 && astarmatrix[i][j].isobstacle != 1)
    		{
    			printf("%c",astarmatrix[i][j].isobstacle);
    		}
    		else
    		{
    			printf("%d",astarmatrix[i][j].isobstacle);
    		}
    		
    	}
    	printf("\n");
    }
 #endif
	return 0;
}
#endif