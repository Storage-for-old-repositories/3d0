#pragma once

#include <string>
#include <memory>
#include <map>

#include "../Renderer/ShaderProgram.h"

/**
 * Вероятно тут должен быть синглтон
 */

class ResourcesManager
{
private:
  typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
  ShaderProgramsMap m_shaderPrograms;

  std::string m_path;

  std::string getStringFromFile(const std::string &relativePath) const;

public:
  ResourcesManager(const std::string &execPath);
  ~ResourcesManager() = default;

  // Конструктор копирования
  ResourcesManager(const ResourcesManager &) = delete;

  // Конструктор перемещение
  ResourcesManager(ResourcesManager &&) = delete;

  // Оператор присваивания
  ResourcesManager &operator=(const ResourcesManager &) = delete;

  // Оператор перемещения
  ResourcesManager &operator=(ResourcesManager &&) = delete;

  std::shared_ptr<Renderer::ShaderProgram> loadShaders(
      const std::string &name,
      const std::string &vertPath,
      const std::string &fragPath);

  std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string &name);
};
