
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  while (!glfwWindowShouldClose(pWindow))
  {

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(pWindow);

    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
