#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
  GLuint ID;

  void getSourceFromFile(const char* file_path, std::string& output_str);

public:
  Shader(const char* vertex_path, const char* fragment_path);
  ~Shader();

  void use();
  
  void u3f(const char* name, const float x, const float y, const float z);
  void u3f(const char* name, float vec[3]);
  void mat4f(const char* name, glm::mat4 &matrix);
};

