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

#include <vector>

class edge_rasterizer {
public:
	edge_rasterizer();
	virtual ~edge_rasterizer();
	void init(int x1, int y1, int x2, int y2);
	void init(int x1, int y1, int x2, int y2, int x3, int y3);
	bool more_fragments() const;
	void next_fragment();
	int x() const;
	int y() const;

private:
	bool init_edge(int x1, int y1, int x2, int y2);
	void update_edge();
	bool two_edges;
	int x1; int y1;
	int x2; int y2;
	int x3; int y3;
	int x_start; int y_start;
	int x_stop; int y_stop;
	int x_current; int y_current;
	int dx; int dy;
	int x_step; int y_step;
	int Numerator;
	int Denominator;
	int Accumulator;
	bool valid;

};

edge_rasterizer::edge_rasterizer() : valid(false)
{}
edge_rasterizer::~edge_rasterizer()
{}

void edge_rasterizer::init(int x1, int y1,
	int x2, int y2)
{
	this->two_edges = false;
	this->x1 = x1; this->y1 = y1;
	this->x2 = x2; this->y2 = y2;
	this->init_edge(x1, y1, x2, y2);
}

void edge_rasterizer::init(int x1, int y1,
	int x2, int y2,
	int x3, int y3)
{
	this->two_edges = true;
	this->x1 = x1; this->y1 = y1;
	this->x2 = x2; this->y2 = y2;
	this->x3 = x3; this->y3 = y3;
	bool horizontal = !(this->init_edge(x1, y1, x2, y2));
	if (horizontal) {
		this->two_edges = false;
		this->init_edge(x2, y2, x3, y3);
	}
}

bool edge_rasterizer::init_edge(int x1, int y1,
	int x2, int y2)
{
	this->x_start = x1; this->y_start = y1;
	this->x_stop = x2; this->y_stop = y2;
	this->x_current = this->x_start;
	this->y_current = this->y_start;
	int dx = this->x_stop - this->x_start;
	int dy = this->y_stop - this->y_start;
	this->x_step = (dx < 0) ? -1 : 1; this->y_step = 1;
	this->Numerator = std::abs(dx); // Numerator = |dx|
	this->Denominator = std::abs(dy); // Assumption: dy > 0
	this->Accumulator = (x_step > 0) ? Denominator : 1;
	return (this->valid = (this->y_current < this->y_stop));
}

bool edge_rasterizer::more_fragments() const
{
	return this->valid;
}

void edge_rasterizer::next_fragment()
{
	this->y_current += this->y_step;
	if (this->y_current < this->y_stop)
		this->update_edge();
	else {
		if (this->two_edges) {
			this->init_edge(x2, y2, x3, y3);
			this->two_edges = false;
		}
	}
	this->valid = (this->y_current < this->y_stop);
}

void edge_rasterizer::update_edge()
{
	this->Accumulator += this->Numerator;
	while (this->Accumulator > this->Denominator) {
		this->x_current += this->x_step;
		this->Accumulator -= this->Denominator;
	}
}

int edge_rasterizer::x() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"edge_rasterizer::x(): Invalid State"
			);
	}
	return this->x_current;
}

int edge_rasterizer::y() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"edge_rasterizer::y(): Invalid State"
			);
	}
	return this->y_current;
}

class triangle_rasterizer {
public:
	triangle_rasterizer(int x1, int y1, int x2, int y2, int x3, int y3);
	virtual ~triangle_rasterizer();
	void init(int x1, int y1, int x2, int y2, int x3, int y3);
	bool more_fragments() const;
	void next_fragment();
	int x() const;
	int y() const;
	
private:
	void initialize_triangle(int x1, int y1,
		int x2, int y2,
		int x3, int y3);
	int LowerLeft();
	int UpperLeft();
	glm::ivec2 ivertex[3];
	int lower_left, upper_left, the_other;
	edge_rasterizer leftedge;
	edge_rasterizer rightedge;
	int x_start, y_start;
	int x_stop, y_stop;
	int x_current, y_current;
	bool valid;
};

triangle_rasterizer::triangle_rasterizer(int x1, int y1,
	int x2, int y2,
	int x3, int y3)
	: valid(false)
{
	this->initialize_triangle(x1, y1, x2, y2, x3, y3);
}
triangle_rasterizer::~triangle_rasterizer()
{}

void triangle_rasterizer::init(int x1, int y1, int x2, int y2, int x3, int y3)
{
	glm::vec3 UL = glm::ivec3(x1, x2, 0);
	glm::vec3 LL = glm::ivec3(x1, x3, 0);
	glm::vec3 OT = glm::ivec3(x2, x3, 0);

	glm::vec3 e1 = UL - LL;
	glm::vec3 e2 = OT - LL;
	glm::vec3 e0 = glm::cross(e1, e2);
	int d = e0.z;

	edge_rasterizer left;
	edge_rasterizer right;

	if (LL.y == OT.y){
		// For low horisontal edge
		left.init(LL.x, LL.y, UL.x, UL.y);
		right.init(OT.x, OT.y, UL.x, UL.y);
	}
	else if (UL.y == OT.y){
		// For high horisontal edge
		left.init(LL.x, LL.y, UL.x, UL.y);
		right.init(LL.x, LL.y, OT.x, OT.y);
	}
	else if (d > 0){
		// For two left edges
		left.init(LL.x, LL.y, OT.x, OT.y, UL.x, UL.y);
		right.init(LL.x, LL.y, UL.x, UL.y);
	}
	else if (d < 0){
		// For two right edges
		left.init(LL.x, LL.y, UL.x, UL.y);
		right.init(LL.x, LL.y, OT.x, OT.y, UL.x, UL.y);
	}

	// Continues algorithm
	next_fragment();

}

int triangle_rasterizer::LowerLeft()
{
	int ll = 0;
	for (int i = ll + 1; i < 3; ++i) {
		if ((this->ivertex[i][2] < this->ivertex[ll][2])
			||
			((this->ivertex[i][2] == this->ivertex[ll][2])
			&&
			(this->ivertex[i][1] < this->ivertex[ll][1])
			)
			)
		{
			ll = i;
		}
	}
	return ll;
}

int triangle_rasterizer::UpperLeft()
{
	int ul = 0;
	for (int i = ul + 1; i < 3; ++i) {
		if ((this->ivertex[i][2] > this->ivertex[ul][2])
			||
			((this->ivertex[i][2] == this->ivertex[ul][2])
			&&
			(this->ivertex[i][1] < this->ivertex[ul][1])
			)
			)
		{
			ul = i;
		}
	}
	return ul;
}

void triangle_rasterizer::initialize_triangle(int x1, int y1,
	int x2, int y2,
	int x3, int y3)
{
	this->ivertex[0] = glm::ivec2(x1, y1);
	this->ivertex[1] = glm::ivec2(x2, y2);
	this->ivertex[2] = glm::ivec2(x3, y3);
	this->lower_left = this->LowerLeft();
	this->upper_left = this->UpperLeft();
	this->the_other = 3 - lower_left - upper_left;
	glm::ivec2 ll = this->ivertex[this->lower_left];
	glm::ivec2 ul = this->ivertex[this->upper_left];
	glm::ivec2 ot = this->ivertex[this->the_other];

	// Let e1 be the vector from ’lower_left’ to ’upper_left’
	glm::ivec2 e1(ul - ll);
	// Let e2 be the vector from ’lower_left’ to ’the_other’.
	glm::ivec2 e2(ot - ll);
	// If the cross product (e1 x e2) has a positive
	// z-component then the point ’the_other’ is to
	// the left of e1, else it is to the right of e1.
	int z_component_of_e1xe2 = e1.x * e2.y - e1.y * e2.x;

	if (z_component_of_e1xe2 != 0) {
		if (z_component_of_e1xe2 > 0) { // The RED triangle
			this->leftedge.init(ll.x, ll.y,
				ot.x, ot.y, ul.x, ul.y);
			this->rightedge.init(ll.x, ll.y, ul.x, ul.y);
		}
		else { // The BLUE triangle
			this->leftedge.init(ll.x, ll.y, ul.x, ul.y);
			this->rightedge.init(ll.x, ll.y,
				ot.x, ot.y, ul.x, ul.y);
		}
		this->x_start = this->leftedge.x();
		this->y_start = this->leftedge.y();
		this->x_current = this->x_start;
		this->y_current = this->y_start;
		this->x_stop = this->rightedge.x() - 1;
		this->y_stop = this->ivertex[this->upper_left].y;
		this->valid = (this->x_current <= this->x_stop);
		if (!(this->valid)) {
			this->next_fragment();
		}
	}
}

bool triangle_rasterizer::more_fragments() const
{
	return this->valid;
}

void triangle_rasterizer::next_fragment()
{
	if (this->x_current < this->x_stop) {
		this->x_current += 1;
	}
	else {
		// this->x_current >= this->x_stop, so find the next NonEmptyScanline
		// this->valid = this->SearchForNonEmptyScanline();

		this->leftedge.next_fragment();
		this->rightedge.next_fragment();
		while (this->leftedge.more_fragments() && (leftedge.x() >= rightedge.x())) {
			leftedge.next_fragment();
			rightedge.next_fragment();
		}
		this->valid = this->leftedge.more_fragments();
		if (this->valid) {
			this->x_start = leftedge.x();
			this->x_current = this->x_start;
			this->x_stop = rightedge.x() - 1;
			this->y_current = leftedge.y();
		}
	}
}

int triangle_rasterizer::x() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"triangle_rasterizer::x(): Invalid State"
			);
	}
	return this->x_current;
}

int triangle_rasterizer::y() const
{
	if (!this->valid) {
		throw std::runtime_error(
			"triangle_rasterizer::y(): Invalid State"
			);
	}
	return this->y_current;
}

void triangles2(int x1, int y1, int x2, int y2, int x3, int y3) {
	triangle_rasterizer trian(x1, y1, x2, y2, x3, y3);

	trian.init(x1, y1, x2, y2, x3, y3);

	glm::vec3 UL = glm::ivec3(x1, x2, 0);
	glm::vec3 LL = glm::ivec3(x1, x3, 0);
	glm::vec3 OT = glm::ivec3(x2, x3, 0);



	int leftx = x_start;
	int lefty = y();
	int rightx = x();

	if (leftx < rightx) {
		for (int i = leftx; i < rightx; ++i){
			DotMaker::instance()->drawDot(i, lefty);
		}
	}
	else {
		for (int i = rightx; i < leftx; ++i){
			DotMaker::instance()->drawDot(i, lefty);
		}
		next_fragment();
		next_fragment();
	}
}


void triangles(int x1, int y1, int x2, int y2, int x3, int y3) {
	edge_rasterizer left;
	left.init(x1, y1, x2, y2);

	edge_rasterizer right;
	right.init(x2, y2, x3, y3);

	while (left.more_fragments() && right.more_fragments()) {
		int leftx = left.x();
		int lefty = left.y();
		int rightx = right.x();

		if (leftx < rightx) {
			for (int i = leftx; i < rightx; ++i){
				DotMaker::instance()->drawDot(i, lefty);
				}
		}
		else {
			for (int i = rightx; i < leftx; ++i){
				DotMaker::instance()->drawDot(i, lefty);
			}
			left.next_fragment();
			right.next_fragment();
		}
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

	triangles(1, 1, 5, 5, 5, 1);

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
		"Grafik 2015 framework",
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