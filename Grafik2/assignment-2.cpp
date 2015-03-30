#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "iofiles.h"
#include "glmutils.h"
#include "triangle.h"

/**
 * defines what should be drawn
 * \param GRID - if defined a grid is drawn.
 * \param TESTLINE - if defined the control line is drawn.
 * \param DOTLINE - if defined the approximated line is drawn as dots.
 */
#define GRID 1
#define TESTTRIANGLE 1
#define DOTTRIANGLE 1
 
/**
 * Global Variables
 * \param x_1 - the x-coordinate of the first fixed point of the triangle
 * \param y_1 - the y-coordinate of the first fixed point of the triangle
 * \param x_2 - the x-coordinate of the second fixed point of the triangle
 * \param y_2 - the y-coordinate of the second fixed point of the triangle
 * \param x_3 - the x-coordinate of the varying point of the triangle
 * \param y_3 - the y-coordinate of the varying point of the triangle
 * \param WindowWidth - the current width of the window
 * \param WindowHeight - the current height of the window
 * \param WindowTitle - the current title of the window
 * \param NGridLines - the number of gridlines
 * \param CoordinatesChaged - true if the arrow keya has been pressed, false otherwize
 */
int x_1 = -5;
int y_1 = -6;
int x_2 =  6;
int y_2 =  4;
int x_3 = -2;
int y_3 =  3;

int WindowWidth  = 500;
int WindowHeight = 500;
std::string WindowTitle("Assignment 1: Scanconvertion of lines");

int NGridLines = 21;

bool CoordinatesChanged = false;


/**
 * Clears any OpenGL errors
 */
void ErrorClear(std::string Message = "")
{
    //std::cout << "-->ErrorClear()" << std::endl << std::flush;

    bool BeQuiet = true;

    bool AnyErrors = false;
    GLenum ErrorCode = GL_NO_ERROR;
    std::ostringstream errormessage;
    errormessage << "ErrorClear(): " << Message << ": ";
    while ((ErrorCode = glGetError()) != GL_NO_ERROR) {
	AnyErrors = true;
	errormessage << gluErrorString(ErrorCode);
    }
    if (AnyErrors) {
	if (!BeQuiet) {
	    std::cout << errormessage.str() << ": cleared" << std::endl;
	}
    }

    //std::cout << "<--ErrorClear()" << std::endl << std::flush;
}

/**
 * Checks if any errors occurred during processing of OpenGL requests
 * If an error has occurred an exception is thown
 */
void ErrorCheck(std::string Message = "")
{
    // std::cout << "-->ErrorCheck()" << std::endl << std::flush;

    bool AnyErrors = false;
    GLenum ErrorCode = GL_NO_ERROR;
    std::ostringstream errormessage;
    errormessage << "ErrorCheck(): " << Message << ": ";
    while ((ErrorCode = glGetError()) != GL_NO_ERROR) {
	AnyErrors = true;
	errormessage << gluErrorString(ErrorCode);
    }
    if (AnyErrors) {
	throw std::runtime_error(errormessage.str());
	//std::cout << errormessage.str() << std::endl;
    }

    // std::cout << "<--ErrorCheck()" << std::endl << std::flush;
}

/**
 * Prints out a std::vector<glm::vec3> to an output stream
 * \param s - The output stream that should be printed to
 * \param data - The data that should be printed
 * \return the output stream s
 */
std::ostream& operator<<(std::ostream& s, std::vector<glm::vec3> const& data)
{
    for (int i = 0; i < data.size(); ++i) {
	s << data[i] << std::endl;
    }
    return s;
}

/**
 * Reads the contents of a text file and returns in as a string.
 * \param FileName - A string containing the name of the file to be read.
 * \return A sting containing the contents of the file.
 */
std::string Read(std::string const& FileName)
{
    std::string resultprog;
    InputFile Data(FileName);
    while (!Data.eof()) {
	std::string line;
	std::getline(Data, line);
	if (line.size() > 0) {
	    resultprog += line + '\n';
	}
    }
    if (resultprog.size() <= 0) {
	std::ostringstream errormessage;
	errormessage << "No program found in file: " << FileName << std::endl;
	throw std::runtime_error(errormessage.str().c_str());
    }
    return resultprog;
}

/**
 * Generates quadratic grid
 */
std::vector<glm::vec3> GenerateGridLines()
{
    std::vector<glm::vec3> lines;

    for (float i = -9; i <= 9; i += 1.0f) {
	lines.push_back(glm::vec3(-9.0f, i, -0.05f));
	lines.push_back(glm::vec3( 9.0f, i, -0.05f));

	lines.push_back(glm::vec3(i, -9.0f, -0.05f));
	lines.push_back(glm::vec3(i,  9.0f, -0.05f));
    }
    return lines;
}

/**
 * Creates the a buffer containing the vertices of the ideal triangle
 * \param x_1 - the x-coordinate of the first fixed point of the triangle
 * \param y_1 - the y-coordinate of the first fixed point of the triangle
 * \param x_2 - the x-coordinate of the second fixed point of the triangle
 * \param y_2 - the y-coordinate of the second fixed point of the triangle
 * \param x_3 - the x-coordinate of the varying point of the triangle
 * \param y_3 - the y-coordinate of the varying point of the triangle
 * \return A std::vector which contains the coordinates of the vertices of the triangle
 */
std::vector<glm::vec3> GenererateTestTriangle(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
    std::vector<glm::vec3> vertices;
    
    vertices.push_back(glm::vec3(float(x_1), float(y_1), -0.1f));
    vertices.push_back(glm::vec3(float(x_2), float(y_2), -0.1f));
    vertices.push_back(glm::vec3(float(x_3), float(y_3), -0.1f));
    
    return vertices;
}

/**
 * Scanconverts a triangle, i.e. computes the pixels that represents the approximated triangle.
 * \param x_1 - the x-coordinate of the first vertex.
 * \param y_1 - the y-coordinate of the first vertex.
 * \param x_2 - the x-coordinate of the second vertex.
 * \param y_2 - the y-coordinate of the second vertex.
 * \param x_3 - the x-coordinate of the third vertex.
 * \param y_3 - the y-coordinate of the third vertex.
 * \return A std::vector which contains the coordinates of the pixels of the triangle.
 */
std::vector<glm::vec3> GenerateTrianglePixels(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
    std::vector<glm::vec3> pixels;

    triangle_rasterizer triangle(x_1, y_1, x_2, y_2, x_3, y_3);

#if 0
    while (triangle.more_fragments()) {
	pixels.push_back(glm::vec3(float(triangle.x()), float(triangle.y()), 0.0f));

	triangle.next_fragment();
    }
#else
    pixels = triangle.all_pixels();
#endif

    CoordinatesChanged = false;

    return pixels;
}

/**
 * Callback function for window resize
 * \param Window - A pointer to the window beeing resized
 * \param width - The new width of the window
 * \param height - The new height of the window
 */
void ResizeCallback(GLFWwindow* Window, int width, int height)
{
    //std::cout << "-->ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;
    
    // Set the point size - make the size of the dot be a little smaller than the minimum distance
    // between the grid lines
    WindowWidth  = width;
    WindowHeight = height;
    float dist = std::min(WindowWidth, WindowHeight);
    float pointsize = dist / NGridLines;
    glPointSize(pointsize);

    //std::cout << "<--ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;
}

/**
 * Change the coordinates of a vertex depending on the arrow key and the modifier key
 * \param x - the x-coordinate of the vertex
 * \param y - the y-coordinate of the vertex
 * \param key - the arrow key that was pressed
 * \return x - there is no actual return value but the x-parameter is modified according to the key-value
 * \return y - there is no actual return value but the y-parameter is modified according to the key-value
 */
void ChangeVertex(int& x, int& y, int key)
{
    CoordinatesChanged = true;
    switch (key) {
    case GLFW_KEY_RIGHT:
	++x;
	if (x > 9) x = 9;
	break;
    case GLFW_KEY_UP:
	++y;
	if (y > 9) y = 9;
	break;
    case GLFW_KEY_LEFT:
	--x;
	if (x < -9) x = -9;
	break;
    case GLFW_KEY_DOWN:
	--y;
	if (y < -9) y = -9;
    }
}

/**
 * Callback function for keyboard shortcuts
 * \param Window - the window that created the callback
 * \param key - The key code
 * \param scancode - Don't know right now? It is badly documented in GLFW
 * \param action - Can be any of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
 * \param mods - Modifier keys pressed at the same time, like shift etc.
 */
void KeyboardCallback(GLFWwindow* Window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_RELEASE) return;

    //std::cout << "--->KeyboardCallback(...)" << std::endl;

    if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_ESCAPE) {
	    glfwSetWindowShouldClose(Window, GL_TRUE);
	    CoordinatesChanged = false;
	}
	switch (mode) {
	case GLFW_MOD_SHIFT:
	    ChangeVertex(x_1, y_1, key);
	    break;
	case GLFW_MOD_ALT:
	    ChangeVertex(x_2, y_2, key);
	    break;
	default:
	    ChangeVertex(x_3, y_3, key);
	}
    }

    //std::cout << "<---KeyboardCallback(...)" << std::endl;
}

int main() 
{
    try {
	GLenum Error = GL_NO_ERROR;

	// Initialize glfw
	if (!glfwInit()) {
	    throw std::runtime_error("Glfw Failed to initialize");
	}

	// Set up some hints for the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the actual window
	GLFWwindow* Window;
	Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(), NULL, NULL);
	if (!Window) {
	    throw std::runtime_error("Could not create window");
	}
	if (Window != NULL) {
	    glfwMakeContextCurrent(Window);
	}

	// Setup a callback function for resize of the window
	glfwSetWindowSizeCallback(Window, ResizeCallback);

	// Setup a callback function for keyboard input - pres a key
	glfwSetKeyCallback(Window, KeyboardCallback);

	// Initialize glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
	    std::ostringstream errmess;
	    errmess << "GlfwWindow::Initialize(): "
		    << "GLEW failed to initialize: "
		    << glewGetErrorString(err) << ", (" << err << ") \n"
		    << "Status, Using GLEW version: " << glewGetString(GLEW_VERSION)
		    << std::endl;
	    throw std::runtime_error(errmess.str());
	    std::cout << errmess.str();
	}

	// Clear any system errors if any
	ErrorClear("Right after glewInit()");

	// Initialize OpenGL
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // You might change this when the transformations are in place!
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(Window);

	// Read and Compile the vertex program vertexscale.vert
	std::string vertexprogram = Read("vertexscale.vert");
	GLuint vertexprogID = glCreateShader(GL_VERTEX_SHADER);
	if (vertexprogID == 0) {
	    std::ostringstream errormessage;
	    errormessage << "The vertex program could not be created" << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}

	GLchar const* vertexprogramsrc[1];
	vertexprogramsrc[0] = vertexprogram.c_str();
	GLint vertexlengths[1];
	vertexlengths[0] = vertexprogram.size();
    
	glShaderSource(vertexprogID, 1, vertexprogramsrc, vertexlengths);
	glCompileShader(vertexprogID);
    
	GLint vertexsuccess = 0;
	glGetShaderiv(vertexprogID, GL_COMPILE_STATUS, &vertexsuccess);

	if (!vertexsuccess) {
	    GLchar InfoLog[1024] = { 0 };
	    glGetShaderInfoLog(vertexprogID, sizeof(InfoLog), NULL, InfoLog);
	
	    std::ostringstream errormessage;
	    errormessage << "Error compiling vertex program in file: vertexscale.vert: " << InfoLog << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}
	
	// Read and Compile the fragment program linefragment.frag
	std::string linefragmentprogram = Read("linefragment.frag");
	GLuint linefragmentprogID = glCreateShader(GL_FRAGMENT_SHADER);
	if (linefragmentprogID == 0) {
	    std::ostringstream errormessage;
	    errormessage << "The fragment program could not be created" << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}

	GLchar const* linefragmentprogramsrc[1];
	linefragmentprogramsrc[0] = linefragmentprogram.c_str();
	GLint linefragmentlengths[1];
	linefragmentlengths[0] = linefragmentprogram.size();
    
	glShaderSource(linefragmentprogID, 1, linefragmentprogramsrc, linefragmentlengths);
	glCompileShader(linefragmentprogID);
    
	GLint linefragmentsuccess = 0;
	glGetShaderiv(linefragmentprogID, GL_COMPILE_STATUS, &linefragmentsuccess);

	if (!linefragmentsuccess) {
	    GLchar InfoLog[1024] = { 0 };
	    glGetShaderInfoLog(linefragmentprogID, sizeof(InfoLog), NULL, InfoLog);
	
	    std::ostringstream errormessage;
	    errormessage << "Error compiling fragment program in file: linefragment.frag: " << InfoLog << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}

	// Create a lineshader program and Link it with the vertex and linefragment programs
	GLuint lineshaderID = glCreateProgram();
	if (lineshaderID == 0) {
	    std::ostringstream errormessage;
	    errormessage << "Could not create shaderprogram" << std::endl;
	    throw std::runtime_error(errormessage.str());
	}
	glAttachShader(lineshaderID, vertexprogID);
	glAttachShader(lineshaderID, linefragmentprogID);
	
	GLint lineprogramsuccess = 0;
	glLinkProgram(lineshaderID);
	glGetProgramiv(lineshaderID, GL_LINK_STATUS, &lineprogramsuccess);
	if (!lineprogramsuccess) {
	    GLchar InfoLog[1024] = { 0 };
	    glGetProgramInfoLog(lineshaderID, sizeof(InfoLog), NULL, InfoLog);

	    std::ostringstream errormessage;
	    errormessage << "Error linking shaderprogram: " << InfoLog << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}
	glDetachShader(lineshaderID, vertexprogID);
	glDetachShader(lineshaderID, linefragmentprogID);

        // Read and Compile the fragment program dotfragment.frag
	std::string dotfragmentprogram = Read("dotfragment.frag");
	GLuint dotfragmentprogID = glCreateShader(GL_FRAGMENT_SHADER);
	if (dotfragmentprogID == 0) {
	    std::ostringstream errormessage;
	    errormessage << "The fragment program could not be created" << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}

	GLchar const* dotfragmentprogramsrc[1];
	dotfragmentprogramsrc[0] = dotfragmentprogram.c_str();
	GLint fragmentlengths[1];
	fragmentlengths[0] = dotfragmentprogram.size();
    
	glShaderSource(dotfragmentprogID, 1, dotfragmentprogramsrc, fragmentlengths);
	glCompileShader(dotfragmentprogID);
    
	GLint dotfragmentsuccess = 0;
	glGetShaderiv(dotfragmentprogID, GL_COMPILE_STATUS, &dotfragmentsuccess);

	if (!dotfragmentsuccess) {
	    GLchar InfoLog[1024] = { 0 };
	    glGetShaderInfoLog(dotfragmentprogID, sizeof(InfoLog), NULL, InfoLog);
	
	    std::ostringstream errormessage;
	    errormessage << "Error compiling fragment program in file: dotfragment.frag: " << InfoLog << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}

	// Create dotshader program and Link it with the vertex and dotfragment programs
	GLuint dotshaderID = glCreateProgram();
	if (dotshaderID == 0) {
	    std::ostringstream errormessage;
	    errormessage << "Could not create shaderprogram" << std::endl;
	    throw std::runtime_error(errormessage.str());
	}
	glAttachShader(dotshaderID, vertexprogID);
	glAttachShader(dotshaderID, dotfragmentprogID);
	
	GLint dotprogramsuccess = 0;
	glLinkProgram(dotshaderID);
	glGetProgramiv(dotshaderID, GL_LINK_STATUS, &dotprogramsuccess);
	if (!dotprogramsuccess) {
	    GLchar InfoLog[1024] = { 0 };
	    glGetProgramInfoLog(dotshaderID, sizeof(InfoLog), NULL, InfoLog);

	    std::ostringstream errormessage;
	    errormessage << "Error linking shaderprogram: " << InfoLog << std::endl;
	    throw std::runtime_error(errormessage.str().c_str());
	}
	glDetachShader(dotshaderID, vertexprogID);
	glDetachShader(dotshaderID, dotfragmentprogID);

	// Now comes the OpenGL core part

#ifdef GRID
	// This is where the grid is initialized

        // User data
	std::vector<glm::vec3> GridLines;
	GridLines = GenerateGridLines();
	//std::cout << GridLines << std::endl;

        // Make a VertexArrayObject - it is used by the VertexArrayBuffer, and it must be declared!
	GLuint GridVertexArrayID;
	glGenVertexArrays(1, &GridVertexArrayID);
	glBindVertexArray(GridVertexArrayID);

	// Make a GridvertexbufferObject - it uses the previous VertexArrayBuffer!
	GLuint gridvertexbuffer;
	glGenBuffers(1, &gridvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gridvertexbuffer);
	
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, GridLines.size() * sizeof(float) * 3, &(GridLines[0][0]), GL_STATIC_DRAW);

        // Validate the grid shader program
	GLint gridvalidationsuccess = 0;
	glValidateProgram(lineshaderID);
	glGetProgramiv(lineshaderID, GL_VALIDATE_STATUS, &gridvalidationsuccess);
	if (!gridvalidationsuccess) {
	    GLchar InfoLog[1024] = { 0 };
            glGetProgramInfoLog(lineshaderID, sizeof(InfoLog), NULL, InfoLog);

            std::ostringstream errormessage;
            errormessage << "Error validating the line shader program failed: "  << InfoLog << std::endl;
            throw std::runtime_error(errormessage.str().c_str());
	}

	// Get locations of Uniforms
	GLuint linevertexscale   = glGetUniformLocation(lineshaderID, "Scale");
	GLuint linefragmentcolor = glGetUniformLocation(lineshaderID, "Color");	


	// Initialize grid Attributes
	GLuint linearvertexattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
	glVertexAttribPointer(linearvertexattribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif

#ifdef TESTTRIANGLE
	// This is where the TestLine is initialized

        // User data
	std::vector<glm::vec3> TestTriangle;
	TestTriangle = GenererateTestTriangle(x_1, y_1, x_2, y_2, x_3, y_3);
	//std::cout << TestTriangle << std::endl;

	// Make a VertexArrayObject - it is used by the VertexArrayBuffer, and it must be declared!
	GLuint TestTriangleVertexArrayID;
	glGenVertexArrays(1, &TestTriangleVertexArrayID);
	glBindVertexArray(TestTriangleVertexArrayID);

	// Make a VertexBufferObject - it uses the previous VertexArrayBuffer!
	GLuint testtrianglebuffer;
	glGenBuffers(1, &testtrianglebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, testtrianglebuffer);

	// Supply the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, TestTriangle.size() * sizeof(float) * 3, &(TestTriangle[0][0]), GL_STATIC_DRAW);

	// Initialize grid Attributes
	GLuint testtriangleattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
	glVertexAttribPointer(testtriangleattribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif

#ifdef DOTTRIANGLE
	// This where the dots of the lines initialized

	// User data
	std::vector<glm::vec3> TrianglePixels;
	TrianglePixels = GenerateTrianglePixels(x_1, y_1, x_2, y_2, x_3, y_3);
	//std::cout << TrianglePixels << std::endl;

	// Make a VertexArrayObject - it is used by the VertexArrayBuffer, and it must be declared!
	GLuint PixelVertexArrayID;
	glGenVertexArrays(1, &PixelVertexArrayID);
	glBindVertexArray(PixelVertexArrayID);

	// Make a VertexBufferObject - it uses the previous VertexArrayBuffer!
	GLuint dotvertexbuffer;
	glGenBuffers(1, &dotvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dotvertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, TrianglePixels.size() * sizeof(float) * 3, &(TrianglePixels[0][0]), GL_STATIC_DRAW);

        // Validate the dot shader program
	GLint dotvalidationsuccess = 0;
	glValidateProgram(dotshaderID);
	glGetProgramiv(dotshaderID, GL_VALIDATE_STATUS, &dotvalidationsuccess);
	if (!dotvalidationsuccess) {
	    GLchar InfoLog[1024] = { 0 };
            glGetProgramInfoLog(dotshaderID, sizeof(InfoLog), NULL, InfoLog);

            std::ostringstream errormessage;
            errormessage << "Error validating the dot shader program failed: "  << InfoLog << std::endl;
            throw std::runtime_error(errormessage.str().c_str());
	}

	// Get locations of Dot Uniforms
	GLuint dotvertexscale   = glGetUniformLocation(dotshaderID, "Scale");
	GLuint dotfragmentcolor = glGetUniformLocation(dotshaderID, "Color");	

	// Initialize dot Attributes
	GLuint dotvertexattribute = glGetAttribLocation(dotshaderID, "VertexPosition");
	glVertexAttribPointer(dotvertexattribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif

        // Set the point size - make the size of the dot be a little smaller than the minimum distance
	// between the grid lines
	glEnable(GL_PROGRAM_POINT_SIZE);
	float dist = std::min(WindowWidth, WindowHeight);
	float pointsize = dist / NGridLines;
	glPointSize(pointsize);

	// The main loop
	std::cout << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << "* Use the arrow keys to manipulate the vertices of the triangle *" << std::endl;
	std::cout << "* All three vertices can be modified.                           *" << std::endl;
	std::cout << "* It is done with the arrow straitht arrow keys, and also by    *" << std::endl;
	std::cout << "* simultaneously pressing one of the modifier keys:             *" << std::endl;
	std::cout << "* Shift or Alt                                                  *" << std::endl;
	std::cout << "*                                                               *" << std::endl;
	std::cout << "* The Window can be resized using the mouse                     *" << std::endl;
	std::cout << "*                                                               *" << std::endl;
	std::cout << "* Press ESC to finish the program                               *" << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << std::endl;

	while (!glfwWindowShouldClose(Window)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef GRID
	    glUseProgram(lineshaderID);
	    glUniform1f(linevertexscale, 0.1f);
	    glUniform3f(linefragmentcolor, 0.0f, 0.0f, 1.0f);

	    glEnableVertexAttribArray(linearvertexattribute);
	    glBindVertexArray(GridVertexArrayID);   // This is very important! There are two "binds"!
	    glBindBuffer(GL_ARRAY_BUFFER, gridvertexbuffer);
	    glDrawArrays(GL_LINES, 0, GridLines.size());
	    glDisableVertexAttribArray(linearvertexattribute);
	    glUseProgram(0);
#endif

#ifdef TESTTRIANGLE
	    glUseProgram(lineshaderID);
	    glUniform1f(linevertexscale, 0.1f);
	    glUniform3f(linefragmentcolor, 1.0f, 0.0f, 0.0f);

	    glEnableVertexAttribArray(testtriangleattribute);
	    glBindVertexArray(TestTriangleVertexArrayID);   // This is very important! There are two "binds"!
	    glBindBuffer(GL_ARRAY_BUFFER, testtrianglebuffer);
	    if (CoordinatesChanged) {
		TestTriangle = GenererateTestTriangle(x_1, y_1, x_2, y_2, x_3, y_3);
		glBufferData(GL_ARRAY_BUFFER, TestTriangle.size() * sizeof(float) * 3, &(TestTriangle[0][0]),
			     GL_STATIC_DRAW);
	    }
	    glDrawArrays(GL_LINE_LOOP, 0, TestTriangle.size());
	    glDisableVertexAttribArray(testtriangleattribute);
	    glUseProgram(0);
#endif

#ifdef DOTTRIANGLE
	    glUseProgram(dotshaderID);
	    glUniform1f(dotvertexscale, 0.1f);
	    glUniform3f(dotfragmentcolor, 0.0f, 0.0f, 0.0f);

	    glEnableVertexAttribArray(dotvertexattribute);
	    glBindVertexArray(PixelVertexArrayID);   // This is very important! There are two "binds"!
	    glBindBuffer(GL_ARRAY_BUFFER, dotvertexbuffer);
	    if (CoordinatesChanged) {
		TrianglePixels = GenerateTrianglePixels(x_1, y_1, x_2, y_2, x_3, y_3);
		glBufferData(GL_ARRAY_BUFFER, TrianglePixels.size() * sizeof(float) * 3, &(TrianglePixels[0][0]),
			     GL_STATIC_DRAW);
	    }
	    glDrawArrays(GL_POINTS, 0, TrianglePixels.size());
	    glDisableVertexAttribArray(dotvertexattribute);
	    glUseProgram(0);
#endif
	    glfwSwapBuffers(Window);
	    std::stringstream errormessage;
	    errormessage << "End of loop: " << "assignment1.cpp" << ": " << __LINE__ << ": ";
	    ErrorCheck(errormessage.str());
	    glfwPollEvents();
	}
    }
    catch (std::exception const& exception) {
	std::cerr << "Exception: " << exception.what() << std::endl;
    }

    glfwTerminate();

    return 0;
}

