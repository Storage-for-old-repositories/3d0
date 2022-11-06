
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourcesManager.h"

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

int main(int argc, char **argv)
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
  {
    ResourcesManager resourcesManager(argv[0]);
    std::shared_ptr<Renderer::ShaderProgram> pDefaultShaderProgram = resourcesManager.loadShaders(
        "default", "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

    if (!pDefaultShaderProgram)
    {
      std::cerr << "Cat't create shader program" << std::endl;
      return -1;
    }

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

      pDefaultShaderProgram->use();
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(pWindow);

      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}
