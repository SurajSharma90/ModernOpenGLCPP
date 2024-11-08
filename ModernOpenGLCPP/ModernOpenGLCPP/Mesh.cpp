#include "Mesh.h"

Mesh::Mesh(Vertex* vertices, int nrOfVertices)
{
  this->vertices = new Vertex[nrOfVertices];
  this->nrOfVertices = nrOfVertices;

  for (size_t i = 0; i < nrOfVertices; i++)
  {
    this->vertices[i] = vertices[i];
  }

  this->color = glm::vec3(1.f, 1.f, 1.f);
  this->position = glm::vec3(0.f);
  this->rotation = glm::vec3(0.f);
  this->scale = glm::vec3(1.f);
  this->modelMatrix = glm::mat4(1.f);

  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glBindVertexArray(this->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);

  //Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Mesh::~Mesh()
{
  delete [] this->vertices;
}

void Mesh::update()
{
  //Update triangle position
  this->modelMatrix = glm::mat4(1.f);
  this->modelMatrix = glm::translate(this->modelMatrix, this->position);
  this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.x * (float)M_PI / 180.f, glm::vec3(1.f, 0.f, 0.f));
  this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.y * (float)M_PI / 180.f, glm::vec3(0.f, 1.f, 0.f));
  this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation.z * (float)M_PI / 180.f, glm::vec3(0.f, 0.f, 1.f));
  this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
}

void Mesh::render(Shader& shader)
{
  shader.use();

  shader.u3f("vColor", glm::value_ptr(this->color));
  shader.mat4f("modelMatrix", this->modelMatrix);

  glBindVertexArray(this->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
