##VAO VBO EBO opengl模块的核心

VAO：绑定顶点前的一个初始化操作，是必须的，像一个通知GPU开始工作的命令，正确解释应该是方便切换
VBO：顶点缓存的绑定
EBO：顶点索引的绑定

//关于纹理的值的计算：0-1，1代表256，0代表0，则0.5代表256*0.5

##GLSL语言：

//OpenGL希望在每次顶点着色器运行后，我们可见的所有顶点都为标准化设备坐标(Normalized Device Coordinate, NDC)。
//也就是说，每个顶点的x，y，z坐标都应该在-1.0到1.0之间，超出这个坐标范围的顶点都将不可见。

##坐标系统
观察坐标（view）：我们站在电脑前看到的图像
透视坐标(projection)：裁剪空间，根据角度和距离裁剪
世界坐标（model）：我们希望的实体坐标，物体坐标

##齐次坐标
August Ferdinand Möbius 提出的齐次坐标（Homogeneous coordinates）
让我们能够在投影空间里进行图像和几何处理，齐次坐标用 N + 1个分量来描述 N 维坐标。
比如，2D 齐次坐标是在笛卡尔坐标(X, Y)的基础上增加一个新分量 w，变成(x, y, w)，
其中笛卡尔坐标系中的大X，Y 与齐次坐标中的小x，y有如下对应关系：
X = x/w
Y = y/w 

换句话说，齐次坐标描述缩放不变性（scale invariant）。

##法线矩阵
在渲染管线中，模型的坐标会从局部空间(Local space)经Model matrix(简记为M)变换到世界空间(World space)，从世界空间经View matrix(简记为V)变换到观察空间(View space，也称为eye space)，然后再经Projection matrix变换到裁剪空间(clip space) (vertex shader要计算出裁剪空间的坐标)，最后经视口变换(viewport transform)变换到屏幕空间。

正确的normal transform matrix为((MV) −1 ) T  

//当描述一个物体的时候，我们可以用这三个分量来定义一个材质颜色(Material Color)：环境光照(Ambient //Lighting)、漫反射光照(Diffuse Lighting)和镜面光照(Specular Lighting)。

ambient材质向量：定义了在环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色。
diffuse材质向量：定义了在漫反射光照下物体的颜色。（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色。
specular材质向量：设置的是镜面光照对物体的颜色影响（或者甚至可能反射一个物体特定的镜面高光颜色）。
shininess：影响镜面高光的散射/半径。

一个三维场景的画面的好坏，百分之四十取决于模型，百分之六十取决于贴图，可见贴图在画面中所占的重要性。

我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，

//vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb
##texture函数返回的是vec4，在glsl语言中，有以下准则：
作为颜色向量时，用rgba表示分量，就如同取数组的中具体数据的索引值。三维颜色向量就用rgb表示分量。比如对于颜色向量vec4 color，color[0]和color.r都表示color向量的第一个值，也就是红色的分量。其他相同。
作为位置向量时，用xyzw表示分量，xyz分别表示xyz坐标，w表示向量的模。三维坐标向量为xyz表示分量，二维向量为xy表示分量。
作为纹理向量时，用stpq表示分量，三维用stp表示分量，二维用st表示分量。也可能是strq，r和rgb中的r有歧义改为p。

###https://blog.csdn.net/xiangyunl/article/details/5673572 opengl学习的网站

##一个可以基础学习图形算法数学相关，底层相关的入口https://zhuanlan.zhihu.com/p/66157419

##关于编译assimp的一个总结：
1，从github上下载源码。可支持的版本目前使3.1.1，再高就是一大堆问题，编译一大堆报错，解决不了
2，编译3.1.1版本要注意把ASSIMP_BUILD_TESTS这个勾给去掉，不然编不过

##关于几何着色器
他是根据提供的点坐标，然后将坐标作为坐标系原点，然后在几何shader中添加的坐标，是根据这个原点来定义的。


##编box2d遇到的问题：
1，fileno 的问题，去掉cmake list里面关于c++11的支持
2，mutex 的问题，还没找到解决办法，mingw的thread也下载不下来，可能需要切换到mingw64.

##关于freetype字体添加的问题：
1，从网上下载freetype的库，mingw用到的是一个include头文件freetype，和一个ft2build.h文件，lib文件是libfreetype.dll.a
2，需要注意的是，在执行的时候需要把freetype.dll放在与执行文件同意目录下，或者放到mingw的bin目录下

##在解bug中遇到一个很奇怪的问题：错误代码：ERROR: 0:? : 'pre-mature EOF' : syntax error syntax error
这个错误没有显示错误的行数，所以一直在当前的shader文件中找不到错误，无意中发现是前面的一个shader文件丢了一个花括号，会导致当前这个错误

##单人游戏和多人游戏的唯一不同在于使用哪个时间片管理器：多人玩家时间片管理器发送指令给主机，主机缓存一批指令然后广播给所有玩家，这样保证所有客户端得到相同批次的指令，保证他们的模拟组件步调一致。单人游戏的时间片管理器发送指令直接到模拟组件。所有的指令在执行之前都是验证狗的，来防止作弊。当AI实现后，AI将发送指令就像人玩家一样。