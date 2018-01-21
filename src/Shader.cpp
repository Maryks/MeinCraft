#include "Shader.h"

Shader::Shader()
{
	this->loadFromFileAndCompile("src/Shaders/vertexShader.glsl", "src/Shaders/fragmentShader.glsl");
	m_locationProjViewMatrix = glGetUniformLocation(m_programID, "projectionViewMatrix");

	std::cout << "Shader created." << std::endl;
}

void Shader::bind()
{
	glUseProgram(m_programID);
}

GLuint Shader::getID()
{
	return m_programID;
}

bool Shader::loadFromFileAndCompile(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexShaderSource);
		fShaderFile.open(fragmentShaderSource);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into (string and then) GLchar array
		vertexShaderCode = vShaderStream.str();
		fragmentShaderCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILES_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}

	const GLchar* vShaderCode = vertexShaderCode.c_str();
	const GLchar* fShaderCode = fragmentShaderCode.c_str();

	// compile vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, 0);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];	// delete infoLog later?
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// compile fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// link program
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glBindAttribLocation(m_programID, 0, "vertexCoordinates");
	glBindAttribLocation(m_programID, 1, "texCoordinates");
	glLinkProgram(m_programID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Shader::setProjectionViewMatrix(glm::mat4 projectionViewMatrix)
{
	glUniformMatrix4fv(m_locationProjViewMatrix, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
}

void Shader::unbind()
{
	glUseProgram(0);
}