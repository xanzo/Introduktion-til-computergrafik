/**
* @Author: Christian Muf
* @Date:   2014-04-09 22:00:24
* @Last Modified time: 2015-02-15 16:34:05
* @Version: 1.3
*/

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

#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
using std::string;
using std::ifstream;

#include "ShaderProgram.h"
#include "DotMaker.h"
/**
* Find the GLFW keycodes here:
* http://www.glfw.org/docs/latest/group__keys.html
*/

class line_rasterizer {
public:
	line_rasterizer();
	virtual ~line_rasterizer();
	void init(int x1, int y1, int x2, int y2);
	bool more_fragments() const;
	void next_fragment();
	int x() const;
	int y() const;
private:
	int x_start;
	int y_start;
	int x_stop;
	int y_stop;
	int x_current;
	int y_current;
	bool left_right;
	int dx;
	int dy;
	int abs_2dx;
	int abs_2dy;
	int d;
	int x_step;
	int y_step;
	bool valid;
	void (line_rasterizer::*innerloop)();
};

line_rasterizer::line_rasterizer() : valid(false)
{}
line_rasterizer::~line_rasterizer()
{}

void line_rasterizer::init(int x1, int y1, int x2, int y2)
{
	this->x_start = x1;
	this->y_start = y1;
	this->x_stop = x2;
	this->y_stop = y2;
	this->x_current = this->x_start;
	this->y_current = this->y_start;

	this->dx = this->x_stop - this->x_start;
	this->dy = this->y_stop - this->y_start;
	this->abs_2dx = std::abs(this->dx) << 1; // 2 * |dx|
	this->abs_2dy = std::abs(this->dy) << 1; // 2 * |dy|
	this->x_step = (this->dx < 0) ? -1 : 1;
	this->y_step = (this->dy < 0) ? -1 : 1;
	if (this->abs_2dx > this->abs_2dy) {
		// The line is x-dominant
		this->left_right = (this->x_step > 0);
		this->d = this->abs_2dy - (this->abs_2dx >> 1);
		this->valid = (this->x_start != this->x_stop);
		this->innerloop =
			&line_rasterizer::x_dominant_innerloop;
	}
	else {
		// The line is y-dominant
		this->left_right = (this->y_step > 0);
		this->d = this->abs_2dx - (this->abs_2dy >> 1);
		this->valid = (this->y_start != this->y_stop);
		this->innerloop =
			&line_rasterizer::y_dominant_innerloop;
	}
}bool line_rasterizer::more_fragments() const
{
	return this->valid;
}void line_rasterizer::next_fragment()
{
	// run the innerloop once
	// Dereference a pointer to a private member function.
	// It looks strange, but it is the way to do it!
	(this->*innerloop)();
}
int line_rasterizer::x() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"line_rasterizer::x(): Invalid State"
			);
	}
	return this->x_current;
}
int line_rasterizer::y() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"line_rasterizer::y(): Invalid State"
			);
	}
	return this->y_current;
}

void line_rasterizer::x_dominant_innerloop()
{
	if (this->x_current == this->x_stop)
		this->valid = false;
	else {
		if ((this->d > 0) ||
			((this->d == 0) && this->left_right)) {
			this->y_current += this->y_step;
			this->d -= this->abs_2dx;
		}
		this->x_current += this->x_step;
		this->d += this->abs_2dy;
	}
}
void line_rasterizer::y_dominant_innerloop()
{
	if (this->y_current == this->y_stop)
		this->valid = false;
	else {
		if ((this->d > 0) ||
			((this->d == 0) && this->left_right)) {
			this->x_current += this->x_step;
			this->d -= this->abs_2dy;
		}
		this->y_current += this->y_step;
		this->d += this->abs_2dx;
	}
}


static void controlScene(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {}
}

static void drawScene(GLuint shaderID)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(-1.0f);
	glDepthFunc(GL_GREATER);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(shaderID);

	DotMaker::instance()->setColor(1.0f, 1.0f, 1.0f);
	DotMaker::instance()->setScene(800, 600, 15, true);
	DotMaker::instance()->setColor(1.0f, 0.0f, 0.0f);
	DotMaker::instance()->drawDot(0, 0);
	DotMaker::instance()->drawDot(1, 1);
	DotMaker::instance()->drawDot(2, 2);
	DotMaker::instance()->drawDot(3, 3);

	glFlush();
}

static int fileRead(std::string const& filename, std::string* result)
{
	std::ifstream ifs(filename.data());
	if (ifs.is_open()) {
		*result = std::string((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		ifs.close();
		return 0;
	}
	return -1;
}

/**
* In case of errors in when running the executable,
* try to use the code that has been commented out.
*/
int main(int argc, char *argv[])
{
	//glewExperimental = true;

	if (!glfwInit())
		return -1;

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLint width = 800;
	GLint height = 600;

	GLFWwindow* window = glfwCreateWindow(
		width, height,
		"Aflevering 1 framework",
		NULL, NULL);

	if (window == NULL) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	std::string vs = "";
	std::string fs = "";

	if (fileRead("Shader.vert", &vs) < 0 ||
		fileRead("Shader.frag", &fs) < 0)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -4;
	}

	glfwSetKeyCallback(window, controlScene);

	GLuint shaderID = ShaderProgram::compileShaderProgram(vs, fs);

	// Create a Vertex Array Object
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		drawScene(shaderID);

		glfwSwapBuffers(window);
	}

	ShaderProgram::deleteShaderPrograms();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
