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
#include "camera.h"

#define HOUSE 1

/**
 * defines what should be drawn
 * \param FIG2_6_27 - if defined Figure 6.27 in Foley is drawn
 * \param FIG2_6_28 - if defined Figure 6.28 in Foley is drawn
 * \param FIG2_6_31 - if defined Figure 6.31 in Foley is drawn
 * \param FIG2_6_34 - if defined Figure 6.34 in Foley is drawn
 */
#define FIG_6_27 1
//#define FIG_6_28 1
//#define FIG_6_31 1
//#define FIG_6_32 1
 
/**
 * Global Variables
 * \param WindowWidth - the current width of the window
 * \param WindowHeight - the current height of the window
 * \param WindowTitle - the current title of the window
 * \param NFigure - the current figure that should be shown
 * \param HouseVertices - the vertices of the house model
 * \param NHouseVertices - the number of entries of the house model
 */
int WindowWidth  = 500;
int WindowHeight = 500;
std::string WindowTitle("Assignment 3: Projection of a House");

int NFigure = 0;

glm::vec3 HouseVertices[] = {
    //Front wall
    glm::vec3( 0.0f,  0.0f, 54.0f),
    glm::vec3(16.0f,  0.0f, 54.0f),
    glm::vec3(16.0f,  0.0f, 54.0f),
    glm::vec3(16.0f, 10.0f, 54.0f),
    glm::vec3(16.0f, 10.0f, 54.0f),
    glm::vec3( 8.0f, 16.0f, 54.0f),
    glm::vec3( 8.0f, 16.0f, 54.0f),
    glm::vec3( 0.0f, 10.0f, 54.0f),
    glm::vec3( 0.0f, 10.0f, 54.0f),
    glm::vec3( 0.0f,  0.0f, 54.0f),
    //Back Wall
    glm::vec3( 0.0f,  0.0f, 30.0f),
    glm::vec3(16.0f,  0.0f, 30.0f),
    glm::vec3(16.0f,  0.0f, 30.0f),
    glm::vec3(16.0f, 10.0f, 30.0f),
    glm::vec3(16.0f, 10.0f, 30.0f),
    glm::vec3( 8.0f, 16.0f, 30.0f),
    glm::vec3( 8.0f, 16.0f, 30.0f),
    glm::vec3( 0.0f, 10.0f, 30.0f),
    glm::vec3( 0.0f, 10.0f, 30.0f),
    glm::vec3( 0.0f,  0.0f, 30.0f),
    //Sides
    glm::vec3( 0.0f,  0.0f, 54.0f),
    glm::vec3( 0.0f,  0.0f, 30.0f),
    glm::vec3(16.0f,  0.0f, 54.0f),
    glm::vec3(16.0f,  0.0f, 30.0f),
    glm::vec3(16.0f, 10.0f, 54.0f),
    glm::vec3(16.0f, 10.0f, 30.0f),
    glm::vec3( 8.0f, 16.0f, 54.0f),
    glm::vec3( 8.0f, 16.0f, 30.0f),
    glm::vec3( 0.0f, 10.0f, 54.0f),
    glm::vec3( 0.0f, 10.0f, 30.0f)
};
int NHouseVertices = sizeof(HouseVertices) / sizeof(glm::vec3);

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
 * Callback function for window resize
 * \param Window - A pointer to the window beeing resized
 * \param width - The new width of the window
 * \param height - The new height of the window
 */
void ResizeCallback(GLFWwindow* Window, int width, int height)
{
    //std::cout << "-->ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;

    //std::cout << "<--ResizeCallback(GLFWwindow* Window, int width, int height)" << std::endl;
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
	}
	else {
	    switch (key) {
	    case '1':
		NFigure = 0;
		break;
	    case '2':
		NFigure = 1;
		break;
	    case '3':
		NFigure = 2;
		break;
	    case '4':
		NFigure = 3;
		break;
	    case '5':
		NFigure = 4;
		break;
	    case '6':
		NFigure = 5;
		break;
	    default:
		std::cout << "No such figure: choosing figure 1" << std::endl;
		NFigure = 0;
	    }
	}
    }

    //std::cout << "<---KeyboardCallback(...)" << std::endl;
}

int main() 
{
    try {
	Camera camera[10];
	camera[0] = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(8.0f, 6.0f, 84.0f),
			   glm::vec2(-50.0f, -50.0f), glm::vec2(50.0f,  50.0f),
			   60.0f, 25.0f);
	camera[1] = Camera(glm::vec3(0.0f, 0.0f, 54.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f),
			   glm::vec3(8.0f, 6.0f, 30.0f), 
			   glm::vec2( -1.0, -1.0), glm::vec2(17.0, 17.0),
			   1.0f, -35.0f);
	camera[2] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(20.0f, 25.0f, 20.0f),
			   glm::vec2(-20.0, -5.0), glm::vec2(20.0,  35.0),
			   1.0f, -35.0f);
	camera[3] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(0.0f, 25.0f, 20.0f * sqrt(2.0)),
			   glm::vec2(-20.0, -5.0), glm::vec2(20.0,  35.0), 
			   1.0f, -35.0f);
	camera[4] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(1.0f, 0.0f, 1.0f), 
			   glm::vec3( -sin(10.0 * M_PI / 180.0), cos(10.0 * M_PI / 180.0), sin(10.0 * M_PI / 180.0)),
			   glm::vec3( 0.0f, 25.0f, 20.0f * sqrt(2.0)), 
			   glm::vec2( -20.0, -5.0), glm::vec2(20.0,  35.0), 
			   1.0f, -35.0f);
	camera[5] = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(8.0f, 6.0f, 84.0f),
			   glm::vec2(-50.0f, -50.0f), glm::vec2(50.0f,  50.0f),
			   60.0f, 25.0f,
			   -1.1f, 0.0f, 0.7f, 0.7f);
	camera[6] = Camera(glm::vec3(0.0f, 0.0f, 54.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f),
			   glm::vec3(8.0f, 6.0f, 30.0f), 
			   glm::vec2( -1.0, -1.0), glm::vec2(17.0, 17.0),
			   1.0f, -35.0f,
			   -0.4f, -0.2f, 0.7f, 0.7f);
	camera[7] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(20.0f, 25.0f, 20.0f),
			   glm::vec2(-20.0, -5.0), glm::vec2(20.0,  35.0),
			   1.0f, -35.0f,
			   0.3f, 0.15f, 0.7f, 0.7f);
	camera[8] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
			   glm::vec3(0.0f, 25.0f, 20.0f * sqrt(2.0)),
			   glm::vec2(-20.0, -5.0), glm::vec2(20.0,  35.0), 
			   1.0f, -35.0f,
			   -1.0f, -0.75f, 0.7f, 0.7f);
	camera[9] = Camera(glm::vec3(16.0f, 0.0f, 54.0f), glm::vec3(1.0f, 0.0f, 1.0f), 
			   glm::vec3( -sin(10.0 * M_PI / 180.0), cos(10.0 * M_PI / 180.0), sin(10.0 * M_PI / 180.0)),
			   glm::vec3( 0.0f, 25.0f, 20.0f * sqrt(2.0)), 
			   glm::vec2( -20.0, -5.0), glm::vec2(20.0,  35.0), 
			   1.0f, -35.0f,
			   0.25f, -0.75f, 0.7f, 0.7f);

	glm::mat4x4 CTM = camera[NFigure].CurrentTransformationMatrix();
	std::cout << "CTM = " << std::endl << CTM << std::endl;

	for (int i = 0; i < NHouseVertices; ++i) {
	    glm::vec4 transformedvertex = CTM * glm::vec4(HouseVertices[i], 1.0f);
	    glm::vec4 transformed = transformedvertex / transformedvertex.w;
	    glm::vec3 euclideanvertex(transformedvertex.x, transformedvertex.y, transformedvertex.z);
	    std::cout << "transformed vertex[" << std::setw(2) << i << "] = [" << euclideanvertex << "]" << std::endl;
	}

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
	std::string vertexprogram = Read("vertextransform.vert");
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

	// Now comes the OpenGL core part

#ifdef HOUSE
	// This is where the house is initialized
        // User data is in the global variable HouseVertices, and the number of entries is in NHousevertices

        // Make a VertexArrayObject - it is used by the VertexArrayBuffer, and it must be declared!
	GLuint HouseVertexArrayID;
	glGenVertexArrays(1, &HouseVertexArrayID);
	glBindVertexArray(HouseVertexArrayID);

	// Make a HousevertexbufferObject - it uses the previous VertexArrayBuffer!
	GLuint housevertexbuffer;
	glGenBuffers(1, &housevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, housevertexbuffer);
	
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, NHouseVertices * 3 * sizeof(float), HouseVertices, GL_STATIC_DRAW);

        // Validate the grid shader program
	GLint housevalidationsuccess = 0;
	glValidateProgram(lineshaderID);
	glGetProgramiv(lineshaderID, GL_VALIDATE_STATUS, &housevalidationsuccess);
	if (!housevalidationsuccess) {
	    GLchar InfoLog[1024] = { 0 };
            glGetProgramInfoLog(lineshaderID, sizeof(InfoLog), NULL, InfoLog);

            std::ostringstream errormessage;
            errormessage << "Error validating the line shader program failed: "  << InfoLog << std::endl;
            throw std::runtime_error(errormessage.str().c_str());
	}

	// Get locations of Uniforms
	GLuint housevertextransform   = glGetUniformLocation(lineshaderID, "CTM");
	GLuint housefragmentcolor     = glGetUniformLocation(lineshaderID, "Color");	


	// Initialize grid Attributes
	GLuint housevertexattribute = glGetAttribLocation(lineshaderID, "VertexPosition");
	glVertexAttribPointer(housevertexattribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif

	// The main loop
	std::cout << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << "* Press the characters: 1, 2, 3, 4, 5, 6                        *" << std::endl;
	std::cout << "* to show the different figures                                 *" << std::endl;
	std::cout << "*                                                               *" << std::endl;
	std::cout << "* The Window can be resized using the mouse                     *" << std::endl;
	std::cout << "*                                                               *" << std::endl;
	std::cout << "* Press ESC to finish the program                               *" << std::endl;
	std::cout << "*****************************************************************" << std::endl;
	std::cout << std::endl;

	while (!glfwWindowShouldClose(Window)) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef HOUSE
	    glUseProgram(lineshaderID);
	    if (NFigure < 5) {
		glm::mat4x4 CTM = camera[NFigure].CurrentTransformationMatrix();
		glUniformMatrix4fv(housevertextransform, 1, GL_FALSE, &CTM[0][0]);
		glUniform3f(housefragmentcolor, 0.0f, 0.0f, 1.0f);

		glEnableVertexAttribArray(housevertexattribute);
		glBindVertexArray(HouseVertexArrayID);   // This is very important! There are two "binds"!
		glBindBuffer(GL_ARRAY_BUFFER, housevertexbuffer);
		glDrawArrays(GL_LINES, 0, NHouseVertices);
		glDisableVertexAttribArray(housevertexattribute);
	    }
	    else {
		for (int cam = 5; cam < 10; ++cam) {
		    glm::mat4x4 CTM = camera[cam].CurrentTransformationMatrix();
		    glUniformMatrix4fv(housevertextransform, 1, GL_FALSE, &CTM[0][0]);
		    glUniform3f(housefragmentcolor, 0.0f, 0.0f, 1.0f);

		    glEnableVertexAttribArray(housevertexattribute);
		    glBindVertexArray(HouseVertexArrayID);   // This is very important! There are two "binds"!
		    glBindBuffer(GL_ARRAY_BUFFER, housevertexbuffer);
		    glDrawArrays(GL_LINES, 0, NHouseVertices);
		}
		glDisableVertexAttribArray(housevertexattribute);
	    }
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

