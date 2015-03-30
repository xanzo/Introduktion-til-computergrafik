/** @file
* @Author: Christian Muf
* @Date:   2015-02-11 13:25:02
* @Last Modified time: 2015-02-20 08:38:51
* @Version: 1.1 (OpenGL 3.3)
*/

#include "DotMaker.h"

#include <string>
using std::string;

#include "ShaderProgram.h"

static GLfloat matrix[16] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f};

static string vs =
"#version 330\n\
layout(location = 0) in vec3 vertPosition; \n\
uniform mat4 uModelMatrix; \n\
void main() { \n\
    gl_Position = uModelMatrix * vec4(vertPosition, 1.0); \n\
}";

static string fs =
"#version 330 \n\
layout(location = 0) out vec4 colourOut; \n\
uniform vec3 uColorVec;\n\
void main() { \n\
    colourOut = vec4(uColorVec, 1.0); \n\
}";

DotMaker::DotMaker()
{
	m_dotParts = NULL;
	m_lineParts = NULL;
	m_numDotParts = 0;
	m_numLineParts = 0;

	m_r = m_g = m_b = 1.0f;

	m_shaderID = ShaderProgram::compileShaderProgram(vs, fs);

	// Generate and bind 2*1 buffer
	glGenBuffers(1, &m_vertexBufferDot);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferDot);
	glGenBuffers(1, &m_vertexBufferLines);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferLines);
}

DotMaker::~DotMaker()
{
	if(m_dotParts != NULL)
		delete m_dotParts;

	if(m_lineParts != NULL)
		delete m_lineParts;
}

DotMaker* DotMaker::instance()
{
	static DotMaker maker = DotMaker();
	return &maker;
}

void DotMaker::setScene(GLuint windowWidth, GLuint windowHeight, GLint radius, bool drawGitter)
{
	static GLint lastRadius = -1;
	if(radius < 0) radius = 0;

	if(radius != lastRadius)
	{
		lastRadius = radius;

		generateDot(radius);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferDot);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numDotParts, m_dotParts, GL_STATIC_DRAW);

		generateGitter(windowWidth, windowHeight, radius);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferLines);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numLineParts, m_lineParts, GL_STATIC_DRAW);
	}

	m_windowWidthHalf = (GLfloat)(windowWidth >> 1);
	m_windowHeightHalf = (GLfloat)(windowHeight >> 1);
	m_radius = radius;

	matrix[0] = 1.0f / m_windowWidthHalf;
	matrix[5] = 1.0f / m_windowHeightHalf;

	glUseProgram(m_shaderID);

	if(drawGitter)
	{
		matrix[12] = (-m_windowWidthHalf) * matrix[0];
		matrix[13] = (-m_windowHeightHalf) * matrix[5];

		// Bind and send points in big point
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferLines);

		glUniform3f(glGetUniformLocation(m_shaderID, "uColorVec"), m_r, m_g, m_b);
		glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "uModelMatrix"), 1, GL_FALSE, matrix);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferLines);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_LINES, 0, m_numLineParts);

		glDisableVertexAttribArray(0);
	}

	// Bind points in big point
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferDot);
}

void DotMaker::drawDot(GLint x, GLint y)
{
	x *= m_radius << 1;
	y *= m_radius << 1;

	matrix[12] = ((GLfloat)x - m_windowWidthHalf) * matrix[0];
	matrix[13] = ((GLfloat)y - m_windowHeightHalf) * matrix[5];

	glUniform3f(glGetUniformLocation(m_shaderID, "uColorVec"), m_r, m_g, m_b);
	glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "uModelMatrix"), 1, GL_FALSE, matrix);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferDot);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_POINTS, 0, m_numDotParts);

	glDisableVertexAttribArray(0);
}

void DotMaker::setColor(GLfloat r, GLfloat g, GLfloat b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void DotMaker::generateDot(GLint radius)
{
	if(m_dotParts != NULL)
		delete m_dotParts;

	GLint diameter = (radius << 1) + 1;
	GLint radiusSquared = radius * radius;

	m_dotParts = new GLfloat[diameter * diameter * 3];

	m_numDotParts = 0;
	for(GLint x = -radius; x <= radius; x++)
	for(GLint y = -radius; y <= radius; y++)
	{
		if(x * x + y * y <= radiusSquared) {
			m_dotParts[m_numDotParts++] = (GLfloat)x;
			m_dotParts[m_numDotParts++] = (GLfloat)y;
			m_dotParts[m_numDotParts++] = 0.1f;
		}
	}
}

void DotMaker::generateGitter(GLuint windowWidth, GLuint windowHeight, GLint radius)
{
	if(m_lineParts != NULL)
		delete m_lineParts;

	GLint radius2 = radius << 1;
	m_lineParts = new GLfloat[(windowWidth + windowHeight) * 6 / radius2];

	m_numLineParts = 0;
	for(unsigned int x = radius2; x < windowWidth; x += radius2) {
		m_lineParts[m_numLineParts++] = (GLfloat)x;
		m_lineParts[m_numLineParts++] = 0.0f;
		m_lineParts[m_numLineParts++] = 0.0f;
		m_lineParts[m_numLineParts++] = (GLfloat)x;
		m_lineParts[m_numLineParts++] = (GLfloat)windowHeight;
		m_lineParts[m_numLineParts++] = 0.0f;
	}

	for(unsigned int y = radius2; y < windowHeight; y += radius2) {
		m_lineParts[m_numLineParts++] = 0.0f;
		m_lineParts[m_numLineParts++] = (GLfloat)y;
		m_lineParts[m_numLineParts++] = 0.0f;
		m_lineParts[m_numLineParts++] = (GLfloat)windowWidth;
		m_lineParts[m_numLineParts++] = (GLfloat)y;
		m_lineParts[m_numLineParts++] = 0.0f;
	}
}