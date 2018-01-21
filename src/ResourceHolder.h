#ifndef RESOURCEHOLDER_H
#define RESOURCEHOLDER_H

#include "FontManager.h"
#include "TextureManager.h"

// make this a singleton?
class ResourceHolder
{
public:
	ResourceHolder();

	void bindTexture(Textures::ID);

	FontManager m_fontMgr;	// implement get function?

private:
	TextureManager m_textureMgr;
};

#endif // !RESOURCEHOLDER_H
