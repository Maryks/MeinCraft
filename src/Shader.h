#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
	Shader();

	void bind();
	GLuint getID();
	bool loadFromFileAndCompile(const std::string&, const std::string&);
	void setProjectionViewMatrix(glm::mat4);
	void unbind();

private:
	GLuint m_programID;
	GLint m_locationProjViewMatrix;
};

#endif // !SHADER_H
