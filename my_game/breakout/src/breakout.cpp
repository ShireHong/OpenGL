#define GLEW_STATIC
#include <GL/glew.h>

//#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "game.h"
#include "resource_manage.h"

//#include <glad/glad.h>
#include <iostream>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Game Breakout(SCR_WIDTH, SCR_HEIGHT); 

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "breakout", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.    

    // glad: load all OpenGL function pointers
    // ---------------------------------------
   

    glfwSetKeyCallback(window, key_callback);
     // OpenGL configuration
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Breakout.init();

    // DeltaTime variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Start Game within Menu State
    Breakout.state = GAME_ACTIVE;


    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        //deltaTime = 0.001f;
        // Manage user input
        Breakout.process_input(deltaTime);

        // Update Game state
        Breakout.update(deltaTime);

        // Render
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.render();

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
            Breakout.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.keys[key] = GL_FALSE;
    }
}
