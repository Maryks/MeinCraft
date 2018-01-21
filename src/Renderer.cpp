#include <SFML\Graphics.hpp>

#include <iostream>

#include "Camera.h"

#include "Renderer.h"

Renderer::Renderer()
	: m_textCounter(0)
{
	std::cout << "Renderer created." << std::endl;
}

void Renderer::addText(const std::string textIn, float xPosition, float yPosition)
{
	m_text.emplace_back();

	m_text.at(m_textCounter).setCharacterSize(20);
	m_text.at(m_textCounter).setFillColor(sf::Color::White);
	m_text.at(m_textCounter).setFont(m_resourceHolder.m_fontMgr.getFont(Fonts::Arial));
	m_text.at(m_textCounter).move(sf::Vector2f(xPosition, yPosition));

	m_text.at(m_textCounter).setString(textIn);
	
	++m_textCounter;
}

void Renderer::addVao(GLuint vao, GLuint vboVert, GLuint vboTex, int vertCount)
{
	m_vao.emplace_back(vao);
	m_vboVert.emplace_back(vboVert);
	m_vboTex.emplace_back(vboTex);
	m_vertCount.emplace_back(vertCount);
}

void Renderer::displayText(sf::RenderWindow& window)
{
	window.pushGLStates();
	window.resetGLStates();
	for (auto text : m_text)
	{
		window.draw(text);
	}
	window.popGLStates();

	this->clearText();
}

void Renderer::draw(Camera& camera)
{
	m_shaderMgr.get(Shaders::Normal).bind();
	m_shaderMgr.get(Shaders::Normal).setProjectionViewMatrix(camera.getProjectionViewMatrix());
	m_resourceHolder.bindTexture(Textures::Sheet);

	std::vector<int>::size_type i = 0;
	for (GLuint vaoID : m_vao)
	{
		glBindVertexArray(vaoID);
		glDrawArrays(GL_QUADS, 0, m_vertCount.at(i++));
		glBindVertexArray(0);
	}

	glUseProgram(0);

	this->clear();
}

void Renderer::clear()
{
	for (GLuint vaoID : m_vao)
	{
		glDeleteVertexArrays(1, &vaoID);
	}
	for (GLuint vboID : m_vboVert)
	{
		glDeleteBuffers(1, &vboID);
	}
	for (GLuint vboID : m_vboTex)
	{
		glDeleteBuffers(1, &vboID);
	}
	m_vao.clear();
	m_vao.shrink_to_fit();
	m_vboVert.clear();
	m_vboVert.shrink_to_fit();
	m_vboTex.clear();
	m_vboTex.shrink_to_fit();

	m_vertCount.clear();
	m_vertCount.shrink_to_fit();
}

void Renderer::clearText()
{
	m_text.clear();
	m_text.shrink_to_fit();

	m_textCounter = 0;
}
