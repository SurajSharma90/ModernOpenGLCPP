#pragma once

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<map>

class ImGuiWrapper
{
private:
  ImGuiIO* io;
  float* bg_color;
  float* color;

public:
  ImGuiWrapper(GLFWwindow* window, const char* glsl_version, float* bg_color, float* color);
  ~ImGuiWrapper();

  void render(GLFWwindow* window);
};

