> assimp是用来在OPENGL中导入3D模型，这个assimp没有现成的库可以使用，需要自己下载源代码编译，网上提供的教程基本上是以VS为主，还有QT版本的，使用MingW编译的很少，至少我没有找到现有的教程，所以自己摸索，使用MingW32编译成功了，大致步骤如下。

## 编译环境：Win10
## 编译工具：Cmake和mingw32

### Github源码

- 网络上教程以及Opengl的教程都是使用assimp的3.1.1的版本，我使用了这个版本进行编译，会报`cppunit/config-auto.h`的错误，这个模块是在test中，如果在cmake中可以直接去掉test编译。

- test模块不能编译过，查看了assimp的源码issue，别人也提过这个问题，貌似在后续的版本中有修复这个问题，所以就开始追踪后续的版本，如果直接跳到最新的版本也是有问题的，Mingw32无法支持一些c++ 11的特性，可能需要适应Mingw64来编译。总之，一个版本一个版本的测试，发现4.0.0版本可以编译完整的，不需要去掉test。需要修改的地方也不多。

	- min和max报错的问题，这个问题在issue里面也有，就是需要在这两个函数前面加上std：：。

	- TVITEMEXW这个结构体缺少定义的报错问题，这个结构体主要包含在commctrl.h中，而且我下载的mingw中的关于启用这个结构体的代码没有打开，所以需要在commctrl.h中的宏定义打开，如下所示：
	之前的代码：
	```
	#if 0
		#define _WIN32_IE	0x0300
	#endif

	```

	更换为
	
	```
	#if 1
		#define _WIN32_IE	0x0500
	#endif

	```

    - commoncontrols.h 找不到的问题，我只是把这个用commctrl.h替换掉了。之后就没有报错了。

    - timeapi.h报错的问题，这个问题assimp的issue上也有解释，所以直接就跟着改。
	将
	```
	#include<timeapi.h>
	```
	更换为
	```
	#ifdef __MINGW32__
		#include <mmsystem.h>
	#else
 		#include <timeapi.h>
	#endif
	```

	- gtest-port.cc中的CRITICAL_SECTION报错的问题，这个问题issue上也有报，但不知道怎么解决的，可能我没有仔细去看，我借鉴了网上
	[opencv解决方案](https://stackoverflow.com/questions/41930349/opencv-installation-error-while-mingw32-make-on-windows)
	直接截图：
	
    	<img src="https://github.com/ShireHong/OpenGL/blob/master/assimp/CRITICAL_SECTION1.png" width="639" height="228"  
    	alt="图片加载失败时，显示这段字"/>
	
	<img src="https://github.com/ShireHong/OpenGL/blob/master/assimp/CRITICAL_SECTION2.png" width="441" height="98"  
    	alt="图片加载失败时，显示这段字"/>


 ### assimp编译环境

   真正编译的时候会有其他问题，其他问题基本上解决方法比较一致，可以参照网上的解决方案，编译完成后，把lib库放在mingw的库中，就OK了。
