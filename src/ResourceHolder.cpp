#include <iostream>

#include "ResourceHolder.h"

ResourceHolder::ResourceHolder()
{
	std::cout << "ResourceHolder created." << std::endl;
}

void ResourceHolder::bindTexture(Textures::ID id)
{
	m_textureMgr.bind(id);
}