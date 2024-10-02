#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
  //Initialise GLFW
  if (!glfwInit())
  {
    std::cout << "GLFW Init failed!" << "\n";
    glfwTerminate();
    return 1;
  }

  //Setup window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Use 4.6 for latest
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No backwards compatability
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatability

  //Create window
  GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Test Window", NULL, NULL);
  if (!window)
  {
    std::cout << "Failed to create glfw window!" << "\n";
    glfwTerminate();
    return 1;
  }

  //Get buffer size dimensions
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

  //Set context for glew
  glfwMakeContextCurrent(window);

  //Allow modern extensions
  glewExperimental = GL_TRUE;

  //Init glew
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to init glew!" << "\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  //Setup viewport (size of the part we are drawing to in the window)
  glViewport(0, 0, bufferWidth, bufferHeight);

  //Game loop
  while (!glfwWindowShouldClose(window))
  {
    //Get and handle user input events
    glfwPollEvents();

    //Clear buffer (color between 0-1 not 255)
    glClearColor(1.f, 0.f, 0.f, 255.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  //End of program cleanup
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}