#define GLEW_STATIC
#include <GL/glew.h>

//#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "game.h"
#include "resource_manage.h"
#include "cursor.h"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH  = 1344;
const unsigned int SCR_HEIGHT = 960;

Game MyGame(SCR_WIDTH, SCR_HEIGHT); 

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Three kingdoms", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	
	glfwSetWindowPos(window, 50, 50);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.    

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetScrollCallback(window, scroll_callback);
	    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
    glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	
     // OpenGL configuration
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double xx,yy;
	glfwSetCursorPos(window, SCR_WIDTH/2, SCR_HEIGHT/2);
	glfwGetCursorPos(window, &xx, &yy);

    MyGame.Init();

    // DeltaTime variables
    float deltaTime = 0.0f;
	float deltaTime_01 = 0.001f;
    float lastFrame = 0.0f;
	GLboolean  init_flag = GL_TRUE;

    // Start Game within Menu State
    MyGame.State = GAME_ACTIVE;

	//GLFWmonitor winmon = glfwGetPrimaryMonitor();

	/*获取屏幕的像素大小*/
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	std::cout << "Screen size is X = " << mode->width << ", Y = " << mode->height << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        // sync time ,same delta
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;		
        lastFrame = currentFrame;
		
		if(init_flag)
		{
			deltaTime_01 = deltaTime;
			init_flag = GL_FALSE;
		}
		else
		{
		//#if 1
			if(deltaTime > 2 * deltaTime_01)
			{
				deltaTime = deltaTime_01;
			}					
			deltaTime_01 = deltaTime;
		//#endif
		}
		
		
        glfwPollEvents();
		
        deltaTime = 0.01f;
		//cout<<"dt:"<<deltaTime<<"   dt0:"<<deltaTime_01<<endl;
        // Manage user input
        MyGame.ProcessInput(deltaTime);

        // Update Game state
        MyGame.UpDate(deltaTime);

        // Render
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        MyGame.Render(deltaTime);

        glfwSwapBuffers(window);
    }

    // Delete all resources as loaded using the resource manager
    ResourceManager::clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            MyGame.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
        {
            MyGame.Keys[key] = GL_FALSE;
            MyGame.KeysProcessed[key] = GL_FALSE;
        }
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{		
		if (action == GLFW_PRESS)
		{
	        MyGame.MouseButton = GL_TRUE;		
		}
	    else if (action == GLFW_RELEASE)
	    {
	        MyGame.MouseButton = GL_FALSE;
	        MyGame.MouseButtonProcessed = GL_FALSE;
	    }
	}
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	double xx,yy;
	glfwGetCursorPos(window, &xx, &yy);
	if(xx < 0 || xx > SCR_WIDTH || yy < 0|| yy > SCR_HEIGHT)
	{
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if(xx >= 0 && xx <= SCR_WIDTH)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		MyGame.ProcessMouseMove(xx, yy);
	}
	else if(yy >=0 && yy <= SCR_HEIGHT)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		MyGame.ProcessMouseMove(xx, yy);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

