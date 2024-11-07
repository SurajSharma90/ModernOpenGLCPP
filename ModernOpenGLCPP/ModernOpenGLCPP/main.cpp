#include<iostream>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
//Do not include loader.h

#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float TO_RADIANS = 3.14159265f / 180.f;

/*
* 1. Init GLFW (Check for error)
* 2. Set window properties, gl version, profile, forward compat.
* 3. Create GLFW window (Check for error)
* 4. Get buffer size from window
* 5. Set opengl context to glfw window for glew
* 6. Set GLEW experimental option
* 7. Init GLEW (Check for errors)
* 8. Set GL Viewport to buffersize
* 9. Main loop (while window is not closed)
* 9a. Poll for events (glfw)
* 9b. Update game
* 9c. Clear window color buffer
* 9d. Swap window buffers
* 10. Cleanup
*/

int main()
{
  //Variables
  float color[] = {1.f, 0.f, 1.f};
  float position[] = {0.f, 0.f, 0.f};
  float rotation[] = {0.f, 0.f, 0.f};
  float scale[] = { 1.f, 1.f, 1.f };
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
  GLuint triVbo;
  GLuint triVao;

  glm::mat4 modelMatrix(1.f);

  //Initialise GLFW
  if (!glfwInit())
  {
    std::cout << "GLFW Init failed!" << "\n";
    glfwTerminate();
    return 1;
  }

  //Setup window properties
  //Good resource: https://www.glfw.org/docs/3.3/window_guide.html#window_full_screen
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Use 4.6 for latest
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No backwards compatability
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Allow forward compatability
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


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

  //Init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  float bg_color[] {1.f, 0.f, 0.f};
  
  //Shaders
  Shader coreShader("Shaders/vertex.vs", "Shaders/fragment.fs");

  //Init triangle
  glGenVertexArrays(1, &triVao);
  glGenBuffers(1, &triVbo);
  glBindVertexArray(triVao);

  glBindBuffer(GL_ARRAY_BUFFER, triVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //Game loop
  while (!glfwWindowShouldClose(window))
  {
    //Get and handle user input events
    glfwPollEvents();

    //Update triangle position
    modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position[0], position[1], position[2]));
    modelMatrix = glm::rotate(modelMatrix, rotation[0] * TO_RADIANS, glm::vec3(1.f, 0.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, rotation[1] * TO_RADIANS, glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::rotate(modelMatrix, rotation[2] * TO_RADIANS, glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale[0], scale[1], scale[2]));

    //Clear buffer (color between 0-1 not 255)
    glClearColor(bg_color[0], bg_color[1], bg_color[2], 255.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Render stuff
    coreShader.use();

    //Uniforms
    coreShader.u3f("vColor", color);
    coreShader.mat4f("modelMatrix", modelMatrix);

    //Triangle
    glBindVertexArray(triVao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //Imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Create Imgui box
    ImGui::Begin("My Name is window");
    ImGui::Text("Hello!");
    ImGui::ColorPicker3("Colors", bg_color);
    ImGui::Spacing();
    ImGui::ColorPicker3("TriColor", color);
    ImGui::Spacing();
    ImGui::SliderFloat3("Position", position, -1.f, 1.f);
    ImGui::Spacing();
    ImGui::SliderFloat3("Rotation", rotation, 0.f, 360.f);
    ImGui::Spacing();
    ImGui::SliderFloat3("Scale", scale, 0.f, 1.f);
    ImGui::End();

    //Draw imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //End gl draw
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}