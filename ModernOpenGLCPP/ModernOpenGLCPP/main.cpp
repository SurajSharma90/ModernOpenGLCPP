#include<iostream>

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
//Do not include loader.h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

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
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
  GLuint triVbo;
  GLuint triVao;

  //Shader source
  GLuint vertShader;
  GLuint fragShader;
  GLuint shaderProgram;
  const char* vertShaderSrc = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
  const char* fragShaderSrc = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 vColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(vColor, 1.0f);\n"
    "}\0";

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

  //Init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  float bg_color[] {1.f, 0.f, 0.f};
  
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

  //Init shaders
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
  glCompileShader(vertShader);

  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragShader);

  //Compile status shaders
  int  success;
  char infoLog[512];
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //Shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  //Error check program
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  //Cleanup shader
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  //Game loop
  while (!glfwWindowShouldClose(window))
  {
    //Get and handle user input events
    glfwPollEvents();

    //Clear buffer (color between 0-1 not 255)
    glClearColor(bg_color[0], bg_color[1], bg_color[2], 255.f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Render stuff
    glUseProgram(shaderProgram);

    glUniform3f(glGetUniformLocation(shaderProgram, "vColor"), color[0], color[1], color[2]);
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
    ImGui::ColorPicker3("TriColor", color);
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