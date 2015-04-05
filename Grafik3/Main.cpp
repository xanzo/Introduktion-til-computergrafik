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

#include "glm\glm.hpp"

#include "camera.h"

glm::vec3 House[] = {
	//Front wall
	glm::vec3(0.0f, 0.0f, 54.0f),
	glm::vec3(16.0f, 0.0f, 54.0f),
	glm::vec3(16.0f, 0.0f, 54.0f),
	glm::vec3(16.0f, 10.0f, 54.0f),
	glm::vec3(16.0f, 10.0f, 54.0f),
	glm::vec3(8.0f, 16.0f, 54.0f),
	glm::vec3(8.0f, 16.0f, 54.0f),
	glm::vec3(0.0f, 10.0f, 54.0f),
	glm::vec3(0.0f, 10.0f, 54.0f),
	glm::vec3(0.0f, 0.0f, 54.0f),
	//Back Wall
	glm::vec3(0.0f, 0.0f, 30.0f),
	glm::vec3(16.0f, 0.0f, 30.0f),
	glm::vec3(16.0f, 0.0f, 30.0f),
	glm::vec3(16.0f, 10.0f, 30.0f),
	glm::vec3(16.0f, 10.0f, 30.0f),
	glm::vec3(8.0f, 16.0f, 30.0f),
	glm::vec3(8.0f, 16.0f, 30.0f),
	glm::vec3(0.0f, 10.0f, 30.0f),
	glm::vec3(0.0f, 10.0f, 30.0f),
	glm::vec3(0.0f, 0.0f, 30.0f),
	//Sides
	glm::vec3(0.0f, 0.0f, 54.0f),
	glm::vec3(0.0f, 0.0f, 30.0f),
	glm::vec3(16.0f, 0.0f, 54.0f),
	glm::vec3(16.0f, 0.0f, 30.0f),
	glm::vec3(16.0f, 10.0f, 54.0f),
	glm::vec3(16.0f, 10.0f, 30.0f),
	glm::vec3(8.0f, 16.0f, 54.0f),
	glm::vec3(8.0f, 16.0f, 30.0f),
	glm::vec3(0.0f, 10.0f, 54.0f),
	glm::vec3(0.0f, 10.0f, 30.0f)
};

void Eksempel1() {
	glm::vec3 vrp(0.0f, 0.0f, 0.0f);
	glm::vec3 vpn(0.0f, 0.0f, 1.0f);
	glm::vec3 vup(0.0f, 1.0f, 0.0f);
	glm::vec3 prp(8.0f, 6.0f, 84.0f);
	glm::vec2 lower_left(-50.0f, -50.0f);
	glm::vec2 upper_right(50.0f, 50.0f);
	float front_plane = 60.0f;
	float back_plane = 25.0f;
}

/**
 * Find the GLFW keycodes here:
 * http://www.glfw.org/docs/latest/group__keys.html
 */
static void controlScene(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	else if(key == GLFW_KEY_1 && action == GLFW_PRESS) {}
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
	if(ifs.is_open()) {
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

	if(!glfwInit())
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
		"Grafik 2015 framework",
		NULL, NULL);
	
	if(window == NULL) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);
	
	if(glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	std::string vs = "";
	std::string fs = "";

	if(fileRead("Shader.vert", &vs) < 0 ||
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

	while(!glfwWindowShouldClose(window))
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
