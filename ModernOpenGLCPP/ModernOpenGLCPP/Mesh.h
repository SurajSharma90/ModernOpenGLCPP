#pragma once

#include "Shader.h"
#include "Vertex.h"

class Mesh
{
private:
  Vertex* vertices;
  int nrOfVertices;

  GLuint VAO;
  GLuint VBO;

  glm::mat4 modelMatrix;
  glm::mat4 projectionMatrix;

  glm::vec3 color;
  bool useVertexColor;

  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

public:
  Mesh(Vertex* vertices, int nrOfVertices);
  ~Mesh();

  void update();
  void render(Shader& shader, glm::mat4& projectionMatrix);

  inline float* getColor() { return glm::value_ptr(this->color); }
  inline float* getPosition() { return glm::value_ptr(this->position); }
  inline float* getRotation() { return glm::value_ptr(this->rotation); }
  inline float* getScale() { return glm::value_ptr(this->scale); }
  inline bool& getUseVertexColor() { return this->useVertexColor; }
};

