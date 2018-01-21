#ifndef RENDERER_H
#define RENDERER_H

#include <GL\glew.h>

#include <vector>

#include "ResourceHolder.h"
#include "ShaderManager.h"
#include "typedefs.h"

class Camera;

class Renderer
{
public:
	Renderer();

	void addText(const std::string, float, float);
	void addVao(GLuint, GLuint, GLuint, int);
	void displayText(sf::RenderWindow&);
	void draw(Camera&);
		
private:
	void clear();
	void clearText();

	ResourceHolder m_resourceHolder;
	ShaderManager m_shaderMgr;

	std::vector<GLuint> m_vao;
	std::vector<GLuint> m_vboVert;
	std::vector<GLuint> m_vboTex;
		
	std::vector<sf::Text> m_text;
	std::vector<int> m_vertCount;

	int m_textCounter;
};

#endif // !RENDERER_H
