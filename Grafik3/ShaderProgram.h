/** @file
* @Author: Christian Muf
* @Date:   2014-03-18 19:45:53
* @Last Modified time: 2014-08-22 23:13:37
* @Version: 1.1
*/

#ifndef GRAPHICS_SHADERPROGRAM_H
#define GRAPHICS_SHADERPROGRAM_H

#if defined(WIN32) || defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#include <OpenGL/glew.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <string>
#include <vector>

class ShaderProgram
{
	private:
		ShaderProgram();
		//ShaderProgram(const ShaderProgram& program) = delete;
		//ShaderProgram& operator=(const ShaderProgram&) = delete;

	public:
		~ShaderProgram();

		static GLuint compileShaderProgram(std::string const& vs, std::string const& fs);
		static void deleteShaderPrograms();

	private:
		GLuint getProgram();

		void compileShader(std::string const& vs, std::string const& fs);
		void addShader(std::string const& shaderString, GLenum shaderType);

	private:
		GLuint m_program;

		static std::vector<ShaderProgram*> s_programs;
};

#endif