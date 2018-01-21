#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	std::string path = "src/Shaders/";

	this->load(Shaders::Normal, path + "vertexShader.glsl", path + "fragmentShader.glsl");

	std::cout << "ShaderManager created." << std::endl;
}

Shader& ShaderManager::get(Shaders::ID id)
{
	auto found = m_shaderMap.find(id);
	return *found->second;
}

void ShaderManager::load(Shaders::ID id, std::string& vertexFilename, std::string& fragmentFilename)
{
	auto shader = std::make_unique<Shader>();
	if (!shader->loadFromFileAndCompile(vertexFilename, fragmentFilename))
	{
		std::cout << "ShaderManager: Failed to load from files " + vertexFilename;
		std::cout << " and " + fragmentFilename << std::endl;
	}
	m_shaderMap.insert(std::make_pair(id, std::move(shader)));
}