#include "ImGuiWrapper.h"

ImGuiWrapper::ImGuiWrapper(GLFWwindow* window, const char* glsl_version, float* bg_color, float* color)
{
  //Init IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  this->io = &ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  this->bg_color = bg_color;
  this->color = color;
}

ImGuiWrapper::~ImGuiWrapper()
{
}

void ImGuiWrapper::render(GLFWwindow* window)
{
  //Imgui
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  //Create Imgui box
  ImGui::Begin("My Name is window");
  ImGui::Text("Hello!");
  ImGui::ColorPicker3("Colors", this->bg_color);
  ImGui::ColorPicker3("TriColor", this->color);
  ImGui::End();

  //Draw imgui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
