
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"

GLfloat point[] = {
    0.0f,
    0.5f,
    0.0f,
    0.5f,
    -0.5f,
    0.0f,
    -0.5f,
    -0.5f,
    0.0f,
};

GLfloat colors[] = {
    0.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0,
    0.0f,
    0.0f,
    1.0f,
};

const char *vertexShader =
    "#version 460\n"
    "layout(location = 0) in vec3 vert_position;\n"
    "layout(location = 1) in vec3 vert_color;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   color = vert_color;\n"
    "   gl_Position = vec4(vert_position, 1.0);\n"
    "   \n"
    "   \n"
    "}\n";

const char *fragmentShader =
    "#version 460\n"
    "in vec3 color;\n"
    "out vec4 frag_color;\n"
    "void main()\n"
    "{\n"
    "   frag_color = vec4(color, 1.0);\n"
    "   \n"
    "   \n"
    "   \n"
    "}\n";

int g_windowSizeW = 640;
int g_windowSizeH = 480;

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height)
{
  g_windowSizeW = width;
  g_windowSizeH = height;

  glViewport(0, 0, width, height);

  std::cout << "Resize window: (w = " << width << "; h = " << height << "; )" << std::endl;
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
  std::cout << "Hook key: " << key
            << ", scancode: " << scancode
            << ", action: " << action
            << ", mode: " << mode << std::endl;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }
}

/// nvidia bind
extern "C"
{
  _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int main(void)
{

  if (!glfwInit())
  {

    std::cerr << "glfwInit -> failed" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *pWindow = glfwCreateWindow(g_windowSizeW, g_windowSizeH, "3D0", nullptr, nullptr);
  if (!pWindow)
  {

    std::cerr << "glfwCreateWindow failed" << std::endl;

    glfwTerminate();
    return -1;
  }

  glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
  glfwSetKeyCallback(pWindow, glfwKeyCallback);

  glfwMakeContextCurrent(pWindow);

  if (!gladLoadGL())
  {

    std::cerr << "gladLoadGL - failed" << std::endl;
    return -1;
  }

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glClearColor(0, 1, 0, 1);

  //
  std::string vShader(vertexShader);
  std::string fShader(fragmentShader);

  Renderer::ShaderProgram shaderProgram(vShader, fShader);

  if (!shaderProgram.isCompiled())
  {
    std::cerr << "Can't create shader program!" << std::endl;
    return -1;
  }

  // shader compile (without errors handling)

  // GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  // glShaderSource(vs, 1, &vertexShader, nullptr);
  // glCompileShader(vs);

  // GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fs, 1, &fragmentShader, nullptr);
  // glCompileShader(fs);

  // GLuint sp = glCreateProgram();
  // glAttachShader(sp, vs);
  // glAttachShader(sp, fs);
  // glLinkProgram(sp);

  // glDeleteShader(vs);
  // glDeleteShader(fs);

  // vbo

  GLuint points_vbo = 0;
  glGenBuffers(1, &points_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

  GLuint colors_vbo = 0;
  glGenBuffers(1, &colors_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  // vao

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  while (!glfwWindowShouldClose(pWindow))
  {

    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(pWindow);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
