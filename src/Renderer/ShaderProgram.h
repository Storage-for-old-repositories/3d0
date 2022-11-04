#pragma once

#include <string>
#include <glad/glad.h>

namespace Renderer
{

  class ShaderProgram
  {
  private:
    bool m_isCompiled = false;
    GLuint m_ID = 0;

    bool createShader(const std::string &source, const GLenum shaderType, GLuint &shaderId);

  public:
    ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
    ~ShaderProgram();

    ShaderProgram() = delete;
    ShaderProgram(ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;

    ShaderProgram(ShaderProgram &&shaderProgram) noexcept;
    ShaderProgram &operator=(ShaderProgram &&) noexcept;

    bool use() const;

    bool isCompiled() const noexcept
    {
      return this->m_isCompiled;
    }

  private:
    template <int size = 1024>
    void outputError(GLuint shaderID, const std::string &nameError)
    {
      GLchar infoLog[size];
      glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);

      std::cerr << "ERROR::SHADER: "
                << nameError
                << ": \n"
                << infoLog << std::endl;
    }
  };

}
