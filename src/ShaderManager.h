#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "Shader.h"

namespace Shaders
{
	enum ID
	{
		Normal
	};
}

class ShaderManager
{
public:
	ShaderManager();

	Shader& get(Shaders::ID);

private:
	void load(Shaders::ID, std::string&, std::string&);

	std::map<Shaders::ID, std::unique_ptr<Shader>> m_shaderMap;
};

#endif // !SHADERMANAGER_H
