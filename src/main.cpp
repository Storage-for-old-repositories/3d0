
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

  GLFWwindow *pWindow = glfwCreateWindow(640, 480, "3D0", nullptr, nullptr);
  if (!pWindow)
  {

    std::cerr << "glfwCreateWindow failed" << std::endl;

    glfwTerminate();
    return -1;
  }

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
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(pWindow);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
