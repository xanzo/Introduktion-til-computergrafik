/** @file
* @Author: Christian Muf
* @Date:   2015-02-11 13:25:16
* @Last Modified time: 2015-02-20 08:26:51
* @Version: 1.1 (OpenGL 3.3)
*/

#ifndef DOT_MAKER_H
#define DOT_MAKER_H

#ifdef WIN32
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

class DotMaker
{
	private:
		DotMaker();

	public:
		~DotMaker();

		static DotMaker* instance();

		void setScene(GLuint windowWidth, GLuint windowHeight, GLint radius, bool drawGitter);
		void drawDot(GLint x, GLint y);
		void setColor(GLfloat r, GLfloat g, GLfloat b);

	private:
		void generateDot(GLint radius);
		void generateGitter(GLuint windowWidth, GLuint windowHeight, GLint radius);

	private:
		GLfloat m_windowWidthHalf;
		GLfloat m_windowHeightHalf;
		GLint m_radius;

		GLuint m_shaderID;
		GLuint m_vertexBufferDot;
		GLuint m_vertexBufferLines;

		GLfloat m_r, m_g, m_b;

		GLfloat* m_dotParts;
		int m_numDotParts;

		GLfloat* m_lineParts;
		int m_numLineParts;
};

#endif