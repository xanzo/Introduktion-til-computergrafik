/** @file
* @Author: Christian Muf
* @Date:   2014-03-18 19:45:48
* @Last Modified time: 2015-02-15 17:14:50
* @Version: 1.2
*/

#include "ShaderProgram.h"

#include <stdarg.h>
#include <cstdlib>
#include <fstream>
using std::ifstream;

/**
* Function coded by Jakob Mikael Kruse-Jensen Wrigley
*/
static int filePrint(std::string const& filename, char const* msg, ...)
{
	FILE* pFile;
	va_list ap;
	va_start(ap, msg);

	// "a" can be changed to "w" to overwrite existing files.
	pFile = fopen(filename.data(), "a");
	if(pFile == NULL) {
	    va_end(ap);
	    return -1;
	}

	vfprintf(pFile, msg, ap);
	va_end(ap);
	return 0;
}

std::vector<ShaderProgram*> ShaderProgram::s_programs;

ShaderProgram::ShaderProgram()
{
	m_program = 0;
}

ShaderProgram::~ShaderProgram()
{
	if(m_program != 0)
		glDeleteProgram(m_program);
}

GLuint ShaderProgram::compileShaderProgram(std::string const& vs, std::string const& fs)
{
	ShaderProgram* program = new ShaderProgram();
	program->compileShader(vs, fs);

	s_programs.push_back(program);

	return program->getProgram();
}

void ShaderProgram::deleteShaderPrograms()
{
	for(std::vector<ShaderProgram*>::iterator it = s_programs.begin(); it != s_programs.end(); ++it)
		delete *it;

	s_programs.clear();
}

GLuint ShaderProgram::getProgram()
{
	return m_program;
}

void ShaderProgram::compileShader(std::string const& vs, std::string const& fs)
{
	if(m_program != 0) {
		filePrint("error.txt", "Fatal: Shader program already exists.\n");
		exit(EXIT_FAILURE);
	}

	m_program = glCreateProgram();

	if(m_program == 0) {
		filePrint("error.txt", "Fatal: Error creating shader program.\n");
		exit(EXIT_FAILURE);
	}
	
	addShader(vs, GL_VERTEX_SHADER);
	addShader(fs, GL_FRAGMENT_SHADER);
	
	GLint success;

	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if(success == 0) {
		GLchar errorLog[1024] = { 0 };
		glGetProgramInfoLog(m_program, 1024, NULL, errorLog);
		filePrint("error.txt", "Fatal: Error linking shader program: %s\n", errorLog);
		exit(EXIT_FAILURE);
	}
	
	glValidateProgram(m_program);
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &success);
	if(!success) {
		GLchar errorLog[1024] = { 0 };
		glGetProgramInfoLog(m_program, 1024, NULL, errorLog);
		filePrint("error.txt", "Fatal: Invalid shader program: %s\n", errorLog);
		exit(EXIT_FAILURE);
	}
}

void ShaderProgram::addShader(std::string const& shaderString, GLenum shaderType)
{
	GLuint shaderObj = glCreateShader(shaderType);
	if(shaderObj == 0) {
		filePrint("error.txt", "Fatal: Error creating shader type %d\n", shaderType);
		exit(EXIT_FAILURE);
	}

	GLchar const* str = shaderString.data();
	GLint length = shaderString.size();
	glShaderSource(shaderObj, 1, &str, &length);
	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar errorLog[1024] = { 0 };
		glGetShaderInfoLog(shaderObj, 1024, NULL, errorLog);
		filePrint("error.txt", "Fatal: Error compiling shader type %d: %s\n", shaderType, errorLog);
		exit(EXIT_FAILURE);
	}

	glAttachShader(m_program, shaderObj);
}
