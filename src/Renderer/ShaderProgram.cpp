#include "ShaderProgram.h"

#include <iostream>

namespace Renderer
{

  ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
  {
    GLuint vertexShaderID;
    if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
    {
      std::cerr << "VERTEX SHADER compile-time error" << std::endl;
      return;
    }

    GLuint fragmentShaderID;
    if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
    {
      std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;

      glDeleteShader(vertexShaderID);
      return;
    }

    m_ID = glCreateProgram();

    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);
    glLinkProgram(m_ID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    GLint success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

    if (!success)
    {
      outputError(m_ID, "Link-time-error");
      return;
    }

    m_isCompiled = true;
  }

  ShaderProgram::~ShaderProgram()
  {
    if (!m_isCompiled)
      return;

    glDeleteProgram(m_ID);
  }

  ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) noexcept
  {
    m_ID = shaderProgram.m_ID;
    m_isCompiled = shaderProgram.m_isCompiled;

    shaderProgram.m_ID = 0;
    shaderProgram.m_isCompiled = false;
  }

  ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) noexcept
  {
    glDeleteProgram(m_ID);

    m_ID = shaderProgram.m_ID;
    m_isCompiled = shaderProgram.m_isCompiled;

    shaderProgram.m_ID = 0;
    shaderProgram.m_isCompiled = false;

    return *this;
  }

  bool ShaderProgram::createShader(const std::string &source, const GLenum shaderType, GLuint &shaderID)
  {
    shaderID = glCreateShader(shaderType);
    const char *code = source.c_str();

    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
      outputError(shaderID, "Compile-time-error");
      return false;
    }

    return true;
  }

  bool ShaderProgram::use() const
  {
    if (!m_isCompiled)
      return false;

    glUseProgram(m_ID);
    return true;
  }
}