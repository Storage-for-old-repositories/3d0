#include "ResourcesManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourcesManager::ResourcesManager(const std::string &execPath)
{
  size_t found = execPath.find_last_of("/\\");
  m_path = execPath.substr(0, found);
}

std::string ResourcesManager::getStringFromFile(const std::string &relativePath) const
{
  std::ifstream file;

  file.open(m_path + "/" + relativePath, std::ios::in | std::ios::binary);

  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << relativePath << std::endl;
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::loadShaders(
    const std::string &name,
    const std::string &vertPath,
    const std::string &fragPath)
{
  std::string vertString = getStringFromFile(vertPath);

  if (vertString.empty())
  {
    std::cerr << "No vetrex shader!" << std::endl;
    return nullptr;
  }

  std::string fragString = getStringFromFile(fragPath);
  if (fragString.empty())
  {
    std::cerr << "No fragment shader!" << std::endl;
    return nullptr;
  }

  std::shared_ptr<Renderer::ShaderProgram> &shader =
      m_shaderPrograms.emplace(
                          name,
                          std::make_shared<Renderer::ShaderProgram>(vertString, fragString))
          .first->second;

  if (shader->isCompiled())
    return shader;

  std::cerr << "Can't load shader program:\n"
            << "Vertex: " << vertPath << "\n"
            << "Fragment: " << fragPath << std::endl;

  return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::getShaderProgram(const std::string &name)
{
  ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(name);

  if (it != m_shaderPrograms.end())
    return it->second;

  return nullptr;
}
