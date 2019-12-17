#include"iostream"

#include<stdlib.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp> // glm::pi

using namespace std;

int main()
{
		
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);//创建一个点
	glm::mat4 trans = glm::mat4(1.0f);//创建单位矩阵
	trans = glm::translate(trans, glm::vec3(1.0, 1.0, 1.0));//设置平移矩阵
	vec = trans * vec;//变换矩阵左乘点向量，获得变换后的点
	cout << vec.x << vec.y << vec.z << endl;
	//system("pause");
	
#if 0
	glm::vec4 Position = glm::vec4( glm::vec3( 0.0 ), 1.0 );
    glm::mat4 Model = glm::mat4( 1.0 );
    Model[3] = glm::vec4( 1.0, 1.0, 0.0, 1.0 );
    glm::vec4 Transformed = Model * Position;
 #endif
}
