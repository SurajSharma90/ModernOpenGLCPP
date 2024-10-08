#include "Shader.h"

void Shader::getSourceFromFile(const char* file_path, std::string& output_str)
{
  std::string code_str;
  std::ifstream shaderFile;

  // ensure ifstream objects can throw exceptions:
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    // open files
    shaderFile.open(file_path);
    std::stringstream shaderStream;
    // read file's buffer contents into streams
    shaderStream << shaderFile.rdbuf();

    // close file handlers
    shaderFile.close();

    // convert stream into string
    code_str = shaderStream.str();

  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE("<< file_path <<")_NOT_SUCCESFULLY_READ" << std::endl;
  }

  output_str = code_str;
}

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
  //Get source
  std::string vertShaderSrc, fragShaderSrc;
  this->getSourceFromFile(vertex_path, vertShaderSrc);
  this->getSourceFromFile(fragment_path, fragShaderSrc);
  const char* vsrc = vertShaderSrc.c_str();
  const char* fsrc = fragShaderSrc.c_str();

  //Init shaders
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vsrc, NULL);
  glCompileShader(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fsrc, NULL);
  glCompileShader(fs);

  //Compile status shaders
  int  success;
  char infoLog[512];
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //Shader program
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vs);
  glAttachShader(this->ID, fs);
  glLinkProgram(this->ID);

  //Error check program
  glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  //Cleanup shader
  glDeleteShader(vs);
  glDeleteShader(fs);
}

Shader::~Shader()
{
  
}

void Shader::use()
{
  //Render stuff
  glUseProgram(this->ID);
}

void Shader::u3f(const char* name, const float x, const float y, const float z)
{
  glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::u3f(const char* name, float vec[3])
{
  glUniform3f(glGetUniformLocation(this->ID, name), vec[0], vec[1], vec[2]);
}
