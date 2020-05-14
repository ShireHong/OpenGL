# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

# 项目信息

project (aircraft)

#CMAKE_SOURCE_DIR 当前的绝对路径
MESSAGE(STATUS "This is CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})

#CMAKE_BINARY_DIR build的绝对路径
MESSAGE(STATUS "This is CMAKE_BINARY_DIR " ${CMAKE_BINARY_DIR})

#CMAKE_CURRENT_BINARY_DIR build的绝对路径
MESSAGE(STATUS "This is CMAKE_CURRENT_BINARY_DIR " ${CMAKE_CURRENT_BINARY_DIR})
#add_subdirectory (include)

#编译需要用的链接库，静态库
set(LIBS glut32.lib 
	libglew32.dll.a 
	libglfw3dll.a 
	libglad.a 
	libopengl32.a
	libfreetype.dll.a
)

#将需要调用到的文件打包到与执行文件一个目录，先把文件重命名
set(RESOURCE ${CMAKE_SOURCE_DIR}/resource)
# 拷贝资源到执行目录下
file(COPY ${RESOURCE} DESTINATION ${CMAKE_BINARY_DIR} )

#添加头文件
include_directories( ${CMAKE_SOURCE_DIR}/include )

#编译可执行文件
aux_source_directory(src DIR_SRCS)

#生成可执行文件
add_executable(aircraft ${DIR_SRCS})

#编译需要链接的库
target_link_libraries(aircraft ${LIBS})